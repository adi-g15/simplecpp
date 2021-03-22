#include "airport.h"

bool getGate(plane* pP){
  for(int i=0;i<nGates;++i)
    if (toGate.at(i)->reserve(pP)){
      pP->gate = i;
      return true;
    }
  return false;
}

const float plane::pts_body[4][2] = {{5,0}, {0, 15}, {20, 0}, {0, -15}};
bool plane::turnoff(int segment){return segment == gate+2;}
bool preLanding(int segment){return segment == -1;}
bool landing(int segment){return segment == 0;}
bool preTakeOff(int segment){return segment == mainPath.size()-2;}
bool takeOff(int segment){return segment == mainPath.size()-1;}

void plane::wakeup(){
  if(timeToSegmentEnd == 0) process_entry_to_next_segment();
  else move_along_current_segment();
}

void plane::move_along_current_segment(){
    //    if((landing(segment) || takeOff(segment)) 
    //&& timeToSegmentEnd == mainPath.at(segment)->delay/2) rwCommon->release();
    forward(stepsize);
    --timeToSegmentEnd;
  sq.log()<< "origin: " << origin.getX() << "," << origin.getY()
	  << "  Orientation: " << orientation << "  stepsize: "
	  << stepsize << endl;
    sq.insert(1,this);
}

void plane::process_entry_to_next_segment(){
  if(takeOff(segment)){
    mainPath.at(segment)->release();
    sq.log() << " Plane " << id << " left." << endl;
    hide();
  }
  else{
    if(!mainPath.at(segment+1)->reserve(this)){
      mainPath.at(segment+1)->await(this);
      return;
    }
    if(preLanding(segment)){
      sq.log() << " Plane " << id << " arrives.\n";
      show();
    }
    if(!preLanding(segment)) mainPath.at(segment)->release();
    ++segment;
    enter(mainPath.at(segment));
  }
}

void plane::process_detour_to_gate(){
  if(subsegment == 0){
    mainPath.at(segment)->release();
    ++subsegment;
    enter(toGate.at(gate));
  }
  else if(subsegment == 1){
    ++subsegment;
    sq.log() << " Plane " << id << " at gate " << gate
	     << " will wait for " << serviceT << endl;
    sq.insert(serviceT,this); // wait at terminal
  }
  else if(subsegment == 2){  // start for takeoff
    ++subsegment;
    enter(fromGate.at(gate));
  }
  else if(subsegment == 3){
    if(!mainPath.at(segment+1)->reserve(this)) sq.insert(1,this);
    else {
      toGate.at(gate)->release();
      ++segment;
      enter(mainPath.at(segment));
    }
  }
}

void plane::enter(taxiway* ptw){
  timeToSegmentEnd = ptw->delay;
  setOrigin(ptw->getStart());
  setOrientation(ptw->getAbsOrientation());
  stepsize = ptw->getLength()/timeToSegmentEnd;
  sq.log()<< "taxiway length: " << ptw->getLength() << "taxiway delay: "
	  << ptw->delay << endl;
  sq.log()<< "origin: " << origin.getX() << "," << origin.getY()
	  << "  Orientation: " << orientation << "  stepsize: "
	  << stepsize << endl;
  sq.insert(0,this);
  return;
}
