#include "airport.h"

void plane::prepareToEnterSegment() {
    if (segment == preLanding)
        land();
    else if (segment == rightTaxiway)
        requestGate();
    else if (segment == rightTaxiway + 1 + gate)
        turnToGate();
    else if (segment == toGates + gate)
        atGate();
    else if (segment == fromGates + gate)
        backOnTaxiway();
    else if (segment == leftTaxiway)
        prepareToTakeOff();
    else if (segment == takeOff)
        depart();
    else
        ordinarySegment();
}

void plane::land() {
    taxiways[landing]->acquire([=]() {
        taxiways[halfRW]->acquire([=]() {
            sim::log() << "Plane " << id << " lands. scheduled arrival "
                       << arrivalT << ", Service time " << serviceT << endl;
            show();
            enter(landing);
        });
    });
}

void plane::enter(int newSegment) {
    segment = newSegment;
    Position linestart = taxiways[segment]->getStart();
    moveTo(linestart.getX(), linestart.getY());
    Position lineend = taxiways[segment]->getEnd();
    face(lineend.getX(), lineend.getY());
    timeToSegmentEnd = taxiways[segment]->traversalT;
    sim::post(0, [=]() { this->moveOnSegment(); });
}

void plane::moveOnSegment() {
    wait(0.01);
    if (timeToSegmentEnd != 0) {
        if ((segment == landing || segment == takeOff) &&
            timeToSegmentEnd == taxiways[segment]->traversalT / 2) {
            taxiways[halfRW]->release();
        }
        forward(taxiways[segment]->stepsize);
        --timeToSegmentEnd;
        sim::post(1, [=]() { moveOnSegment(); });
    } else
        prepareToEnterSegment();
}

void plane::requestGate() {
    for (int i = 0; i < nGates; ++i)
        if (taxiways[toGates + i]->reserve()) {
            gate = i;
            break;
        }
    if (gate < 0)
        sim::post(1, [=]() { requestGate(); });
    else
        taxiways[segment + 1]->acquire([=]() {
            taxiways[segment]->release();
            enter(segment + 1);
        });
}

void plane::turnToGate() {
    taxiways[segment]->release();
    enter(toGates + gate);
}

void plane::atGate() {

    sim::log() << " Plane " << id << " at gate " << gate << " will wait for "
               << serviceT << endl;
    sim::post(serviceT, [=]() { // wait for service
        enter(fromGates + gate);
    });
}

void plane::backOnTaxiway() {
    taxiways[rightTaxiway + 2 + gate]->acquire([=]() {
        taxiways[toGates + gate]->release();
        enter(rightTaxiway + 2 + gate);
    });
}

void plane::prepareToTakeOff() {
    taxiways[takeOff]->acquire([=]() {
        taxiways[halfRW]->acquire([=]() {
            taxiways[segment]->release();
            enter(segment + 1);
        });
    });
}

void plane::depart() {
    taxiways[segment]->release();
    hide();
    sim::log() << " Plane " << id << " left." << endl;
    delete this;
    //  sim::post(0,[=](){delete this;});
}

void plane::ordinarySegment() {
    taxiways[segment + 1]->acquire([=]() {
        taxiways[segment]->release();
        enter(segment + 1);
    });
}
