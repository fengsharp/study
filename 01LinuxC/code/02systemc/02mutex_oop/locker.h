#ifndef LOCKER_H_
#define LOCKER_H_

struct _Locker;
typedef struct _Locker Locker;

typedef void (*LockerLockFunc)(Locker* thiz);
typedef void (*LockerUnlockFunc)(Locker* thiz);
typedef void (*LockerDestroy)(Locker* thiz);

struct _Locker {
    LockerLockFunc lockerFunc;
    LockerUnlockFunc unlockFunc;
    LockerDestroy destroyFunc;

    char priv[0];
};

static inline void locker_lock(Locker* thiz) {
    thiz->lockerFunc(thiz);
}

static inline void locker_unlock(Locker* thiz) {
    thiz->unlockFunc(thiz);
}

static inline void locker_destroy(Locker* thiz) {
    thiz->destroyFunc(thiz);
}

#endif