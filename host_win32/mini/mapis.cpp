#include "mapis.h"
#include <map>
#include "mhostapi.h"
#include "mtypeex.h"

template<typename T> class ManagedObjectPool
{
public:
    int add(T *object)
    {
        mPool[++mIdCount] = object;
        return mIdCount;
    }

    T *get(int id)
    {
        auto it = mPool.find(id);
        return it != mPool.end() ? it->second : nullptr;
    }

    void remove(int id)
    {
        auto it = mPool.find(id);
        if (it != mPool.end())
        {
            mPool.erase(it);
        }
    }

private:
    std::map<int, T *> mPool;
    int mIdCount = 0;
};

static ManagedObjectPool<Gdiplus::Image> *ManagedImagePool()
{
    static ManagedObjectPool<Gdiplus::Image> *pool = nullptr;
    if (!pool)
    {
        pool = new ManagedObjectPool<Gdiplus::Image>;
    }
    return pool;
}

static MImage *CreateImage(MData *data)
{
    if (MDataSize(data) == 0)
    {
        return nullptr;
    }

    //create the memory block.
    HGLOBAL memory = GlobalAlloc(GMEM_FIXED, MDataSize(data));
    if (!memory)
    {
        return nullptr;
    }
    void *memoryBytes = GlobalLock(memory);
    if (memoryBytes)
    {
        memcpy(memoryBytes, MDataBytes(data), MDataSize(data));
    }
    GlobalUnlock(memory);

    //create the stream.
    IStream *stream = nullptr;
    HRESULT streamResult = CreateStreamOnHGlobal(memory, TRUE /* auto free "memory" */, &stream);
    if (streamResult != S_OK)
    {
        GlobalFree(memory);
        return nullptr;
    }

    //create the image.
    Gdiplus::Image *image = Gdiplus::Image::FromStream(stream, FALSE);
    stream->Release();

    int imageId = ManagedImagePool()->add(image);
    return MImageCreate(imageId, [](int id) { ManagedImagePool()->remove(id); });
}

Gdiplus::Image *MManagedImage(int id)
{
    return ManagedImagePool()->get(id);
}

void MRegisterApis()
{
    _MSetApiCreateImage(CreateImage);
}
