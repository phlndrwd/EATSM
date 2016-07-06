#ifndef STATE
#define STATE

#include "Types.h"

class RestartState {
public:
    ~RestartState( );
    static Types::RestartStatePointer Get( );
    
    void AddIndividual( const Types::IndividualPointer );
    
    bool IsRestart( ) const;
    unsigned int GetStartingTimeStep( ) const;
    double GetNutrientVolume( ) const;
    double GetPhytoplanktonVolume( ) const;
    const std::string GetRestartPath( ) const;
    const Types::IndividualMatrix& GetSizeClasses( ) const;
    
    unsigned int GetHeterotrophFrequency( ) const;
    
    void SetIsRestart( const bool );
    void SetStartingTimeStep( const unsigned int );
    void SetNutrientVolume( const double );
    void SetPhytoplanktonVolume( const double );
    void SetRestartPath( const std::string );
    void SetSizeClasses( const Types::IndividualMatrix );
    
private:
    RestartState( );
    
    static Types::RestartStatePointer mThis;
    Types::IndividualMatrix mSizeClasses;
    std::string mRestartPath;
    
    double mPhytoplanktonVolume;
    double mNutrientVolume;
    
    unsigned int mStartingTimeStep;
    unsigned int mHeterotrophFrequency;
    bool mIsRestart;
};

#endif
