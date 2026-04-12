#ifndef IWORLDINTERNAL_H
#define IWORLDINTERNAL_H

namespace internal
{
    class IWorldInternal {
    public:
        virtual ~IWorldInternal() = default;
        virtual bool IsEntityAlive(int e) const = 0;
        virtual void EntityComponentsChanged(int e, int storageId, bool added) = 0;
    };
}

#endif //IWORLDINTERNAL_H