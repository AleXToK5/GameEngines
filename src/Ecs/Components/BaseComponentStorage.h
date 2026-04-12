#ifndef BASECOMPONENTSTORAGE_H
#define BASECOMPONENTSTORAGE_H

#include <span>

class BaseComponentStorage {
public:
    virtual ~BaseComponentStorage() = default;
    
    virtual bool Has(int entityIid) const = 0;
    virtual int Count() const = 0;
    virtual std::span<const int> Entities() const = 0;
    virtual void Remove(int e) = 0;
    virtual int Id() const = 0;
};

#endif //BASECOMPONENTSTORAGE_H