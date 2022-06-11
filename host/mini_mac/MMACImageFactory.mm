#import "MMACImageFactory.h"

//bitmap context:

static CGContextRef CreateBitmapContext(void *bytes, int width, int height) {
    CGColorSpaceRef space = CGColorSpaceCreateDeviceRGB();
    CGContextRef context = CGBitmapContextCreate(
        /* bytesData        */ bytes,
        /* width            */ width,
        /* height           */ height,
        /* bitsPerComponent */ 8,
        /* bytesPerRow      */ width * 4,
        /* spaceColor       */ space,
        /* bitmapInfo       */ kCGBitmapByteOrderDefault | kCGImageAlphaPremultipliedLast
    );
    
    CGColorSpaceRelease(space);
    return context;
}

//image factory:

void MMACImageFactory::install() {
    auto obj = MMACImageFactory::create();
    setInstance(obj);
}

MImage::ptr MMACImageFactory::onDecodeFFData(const MVector<uint8_t>::ptr &ffData) {
    NSData   *nsData  = [NSData dataWithBytes:ffData->data() length:ffData->size()];
    _NSImage *nsImage = [[_NSImage alloc] initWithData:nsData];
    if (!nsImage) {
        return nullptr;
    }
    
    auto image = MMACImage::create();
    image->mNSImage = nsImage;
    return image;
}

MImage::ptr MMACImageFactory::onDecodeBitmap(const MVector<uint8_t>::ptr &bitmap, int width, int height) {
    auto pixels = MVector<uint8_t>::create(); {
        pixels->resize(width * height * 4);
        
        auto src = (MColorRGBA *)bitmap->data();
        auto dst = (MColorABGR *)pixels->data();
        MColorTransform(src, dst, width * height);
    }
    
    _NSImage *nsImage = nil; {
        CGContextRef context = CreateBitmapContext(pixels->data(), width, height);
        CGImageRef   cgImage = CGBitmapContextCreateImage(context);

    #if TARGET_OS_OSX
        nsImage = [[NSImage alloc] initWithCGImage:cgImage size:NSMakeSize(width, height)];
    #elif TARGET_OS_IOS
        nsImage = [[UIImage alloc] initWithCGImage:cgImage];
    #endif

        CGContextRelease(context);
        CGImageRelease(cgImage);
    }

    auto image = MMACImage::create();
    image->mNSImage = nsImage;
    return image;
}

MVector<uint8_t>::ptr MMACImageFactory::onEncodeFFData(const MImage::ptr &image, MImageFileFormat format) {
    _NSImage *nsImage = std::static_pointer_cast<MMACImage>(image)->mNSImage;

    NSData *nsData = nil; {
    #if TARGET_OS_OSX
        NSBitmapImageRep *rep = [NSBitmapImageRep imageRepWithData:nsImage.TIFFRepresentation];
        rep.size = nsImage.size;

        switch (format) {
            case MImageFileFormat::JPEG: nsData = [rep representationUsingType:NSBitmapImageFileTypeJPEG properties:@{}]; break;
            case MImageFileFormat::PNG : nsData = [rep representationUsingType:NSBitmapImageFileTypePNG  properties:@{}]; break;
        }
        
    #elif TARGET_OS_IOS
        switch (format) {
            case MImageFileFormat::JPEG: nsData = UIImageJPEGRepresentation(nsImage, 1); break;
            case MImageFileFormat::PNG : nsData = UIImagePNGRepresentation (nsImage   ); break;
        }
    #endif
    }
    if (!nsData) {
        return nullptr;
    }

    auto ffData = MVector<uint8_t>::create();
    ffData->insert(ffData->end(), (uint8_t *)nsData.bytes, (uint8_t *)nsData.bytes + nsData.length);
    return ffData;
}

MVector<uint8_t>::ptr MMACImageFactory::onEncodeBitmap(const MImage::ptr &image) {
    //new bitmap space:
    _NSImage *nsImage = std::static_pointer_cast<MMACImage>(image)->mNSImage;
    
    auto width  = (int)nsImage.size.width ;
    auto height = (int)nsImage.size.height;

    auto bitmap = MVector<uint8_t>::create();
    bitmap->resize(width * height * 4);

    //draw image on the bitmap context:
    CGContextRef context = CreateBitmapContext(bitmap->data(), width, height);
    
#if TARGET_OS_OSX
    NSGraphicsContext.currentContext = [NSGraphicsContext graphicsContextWithCGContext:context flipped:NO];
    [nsImage drawInRect:NSMakeRect(0, 0, width, height)];
    NSGraphicsContext.currentContext = nil;
#elif TARGET_OS_IOS
    CGContextDrawImage(context, CGRectMake(0, 0, width, height), nsImage.CGImage);
#endif
    
    CGContextRelease(context);

    //transform color format.
    auto src = (MColorABGR *)bitmap->data();
    auto dst = (MColorRGBA *)bitmap->data();
    MColorTransform(src, dst, width * height);

    return bitmap;
}

MSize::ptr MMACImageFactory::onGetPixelSize(const MImage::ptr &image) {
    _NSImage *nsImage = std::static_pointer_cast<MMACImage>(image)->mNSImage;
    return MSize::from(nsImage.size.width, nsImage.size.height);
}
