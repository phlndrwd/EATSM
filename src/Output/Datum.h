#ifndef DATUM
#define	DATUM

#include "Constants.h"
#include "Types.h"

class Datum {
public:

    virtual ~Datum( ) {};
    Constants::eDatumTypes GetType( );
    unsigned GetEnumIndex( );
    const std::string& GetName( );

private:
    virtual void SetType( ) = 0;

protected:
    Constants::eDatumTypes mType;
    unsigned mEnumIndex;
    std::string mName;
};

#endif

