#ifndef INDIVIDUAL
#define	INDIVIDUAL

#include "Constants.h"
#include "Types.h"

class Individual {
public:
    Individual( const double, const unsigned );
    Individual( const Types::GenomePointer, const double, const double, const double, const double );
    Individual( const double, const double, const unsigned );
    ~Individual( );

    Types::IndividualPointer Reproduce( );

    double ConsumePreyVolume( const double, bool isHerbivory = true );
    double Metabolise( const double );
    
    Types::GenomePointer GetGenome( ) const;
    double GetTrophicLevel( ) const;
    unsigned GetSizeClassIndex( ) const;

    unsigned GetAge( ) const;

    bool IsDead( ) const;

    double GetVolumeActual( ) const;
    double GetVolumeHeritable( ) const;
    double GetVolumeMinimum( ) const;
    double GetVolumeReproduction( ) const;
    
    double* GetVolumeActualPointer( ) const;
    double* GetTrophicLevelPointer( ) const;
    
    void SetTrophicLevel( const double );
    void SetSizeClassIndex( const unsigned );

    void SetAge( const unsigned );
    void Kill( );
    
    void SetTag( Types::DataTagPointer );
    void RecordTagData( );

private:
    Types::GenomePointer mGenome;
    Types::DataTagPointer mTag;

    double mVolumeHeritable;
    double mVolumeMinimum;
    double mVolumeReproduction;
    
    double mVolumeActual;
    unsigned mSizeClassIndex;
    double mTrophicLevel;
    unsigned mAge;
    
    bool mIsDead;
};

#endif
