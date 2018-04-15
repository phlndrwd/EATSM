#ifndef INITIALSTATE
#define	INITIALSTATE

#include "Types.h"
#include "RandomSFMT.h"

class InitialState {
public:
    ~InitialState( );
    static Types::InitialStatePointer Get( );

    bool Initialise( const Types::StringMatrix& );

    double& GetNutrientVolume( );
    double& GetAutotrophVolume( );
    Types::IndividualVector& GetHeterotrophs( );
    unsigned& GetInitialPopulationSize( );

private:
    InitialState( );
    static Types::InitialStatePointer mThis;

    Types::IndividualVector mHeterotrophs;
    
    double mNutrientVolume;
    double mAutotrophVolume;
    unsigned mInitialPopulationSize;
};

#endif

