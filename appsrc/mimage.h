#pragma once

#include "mdata.h"
#include "mgeometry.h"

m_enum(MImageFileFormat) {
    JPEG = 1,
    PNG  = 2,
};

m_class(MImage, MObject) {
public:
    static MImage::ptr fromBundle(const std::string &path  );
    static MImage::ptr fromFile  (const std::string &path  );
    static MImage::ptr fromFFData(const MData::ptr  &ffData);
    static MImage::ptr fromBitmap(const MData::ptr  &bitmap, int width, int height);

public:
    void writeFile(const std::string &path, MImageFileFormat format);

    MData::ptr copyFFData(MImageFileFormat format);
    MData::ptr copyBitmap();

    MSize::ptr pixelSize();
};
