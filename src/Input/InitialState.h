#ifndef INITIALSTATE
#define	INITIALSTATE

#include "Types.h"

class InitialState {
public:
    ~InitialState( );
    static Types::InitialStatePointer Get( );

    bool Initialise( const Types::StringMatrix& );
    bool IsInitialised( );

    double& GetNutrientVolume( );
    double& GetAutotrophVolume( );
    Types::IndividualMatrix& GetHeterotrophs( );
    unsigned& GetInitialPopulationSize( );

private:
    InitialState( );
    static Types::InitialStatePointer mThis;

    Types::IndividualMatrix mHeterotrophs;
    
    bool mIsInitialised;
    double mNutrientVolume;
    double mAutotrophVolume;
    unsigned mInitialPopulationSize;
};

#endif

