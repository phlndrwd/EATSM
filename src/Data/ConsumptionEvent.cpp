#include "ConsumptionEvent.h"

ConsumptionEvent::ConsumptionEvent( unsigned timeStep, float preyVolume, float volumeAssimilated, float wasteVolume ) {
    mTimeStep = timeStep;
    mPreyVolume = preyVolume;
    mVolumeAssimilated = volumeAssimilated;
    mWasteVolume = wasteVolume;
}

ConsumptionEvent::~ConsumptionEvent( ) {

}

unsigned ConsumptionEvent::GetTimeStep( ) {
    return mTimeStep;
}

float ConsumptionEvent::GetPreyVolume( ) {
    return mPreyVolume;
}

float ConsumptionEvent::GetVolumeAssimilated( ) {
    return mVolumeAssimilated;
}

float ConsumptionEvent::GetWasteVolume( ) {
    return mWasteVolume;
}
