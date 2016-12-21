#ifndef INITIALSTATE
#define	INITIALSTATE

#include "Types.h"
#include "RandomSFMT.h"

class InitialState {
public:
    ~InitialState( );
    static Types::InitialStatePointer Get( );

    bool Initialise( const Types::StringMatrix& );

    double GetNutrientVolume( ) const;
    double GetAutotrophVolume( ) const;
    Types::IndividualMatrix& GetHeterotrophs( );
    unsigned GetInitialPopulationSize( );

private:
    InitialState( );
    static Types::InitialStatePointer mThis;

    Types::IndividualMatrix mHeterotrophs;
    
    double mNutrientVolume;
    double mAutotrophVolume;
    unsigned mInitialPopulationSize;
};

#endif

