#ifndef CONSUMPTIONEVENT
#define	CONSUMPTIONEVENT

class ConsumptionEvent {
public:
    ConsumptionEvent( unsigned, float, float, float );
    ~ConsumptionEvent( );
    
    unsigned GetTimeStep( );
    float GetPreyVolume( );
    float GetVolumeAssimilated( );
    float GetWasteVolume( );
    
private:
    unsigned mTimeStep;
    float mPreyVolume;
    float mVolumeAssimilated;
    float mWasteVolume;
};

#endif

