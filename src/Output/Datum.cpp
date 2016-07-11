#include "Datum.h"

Constants::eDatumTypes Datum::GetType( ) {
    return mType;
}

unsigned Datum::GetEnumIndex( ) {
    return mEnumIndex;
}

const std::string& Datum::GetName( ) {
    return mName;
}
