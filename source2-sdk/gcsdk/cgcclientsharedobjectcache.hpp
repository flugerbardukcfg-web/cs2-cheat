#pragma once

enum ESOCacheEvent {
     
    eSOCacheEvent_None = 0,

     
    eSOCacheEvent_Subscribed = 1,

     
     
    eSOCacheEvent_Unsubscribed = 2,

     
     
     
    eSOCacheEvent_Resubscribed = 3,

     
     
    eSOCacheEvent_Incremental = 4,

     
     
    eSOCacheEvent_ListenerAdded = 5,

     
     
    eSOCacheEvent_ListenerRemoved = 6,
};

class CGCClientSharedObjectTypeCache;

class CGCClientSharedObjectCache {
   public:
    CGCClientSharedObjectTypeCache* CreateBaseTypeCache(int nClassID);
};
