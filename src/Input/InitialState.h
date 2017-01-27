#ifndef INITIALSTATE
#define	INITIALSTATE

#include "Types.h"
#include "RandomSFMT.h"

class InitialState {
public:
    ~InitialState( );
    static Types::InitialStatePointer Get( );

    void Initialise( const Types::StringMatrix& );

    double& GetNutrientVolume( );
    double& GetAutotrophVolume( );
    Types::IndividualMatrix& GetHeterotrophs( );
    unsigned& GetInitialPopulationSize( );

private:
    InitialState( );
    static Types::InitialStatePointer mThis;

    Types::IndividualMatrix mHeterotrophs;
    
    double mNutrientVolume;
    double mAutotrophVolume;
    unsigned mInitialPopulationSize;
};

#endif

