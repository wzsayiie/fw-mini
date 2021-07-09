#include "mapis.h"
#include <map>
#include "mhostapis.h"
#include "mtypeex.h"

template<typename T> class ManagedPool
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

static ManagedPool<Gdiplus::Image> *ManagedImagePool()
{
    static ManagedPool<Gdiplus::Image> *pool = nullptr;
    if (!pool)
    {
        pool = new ManagedPool<Gdiplus::Image>;
    }
    return pool;
}

static MObject *CreateImage(MObject *a, MObject *)
{
    MData *data = MAsData(a);
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
    _MApiSetFunc(MApi_CreateImage, CreateImage);
}
