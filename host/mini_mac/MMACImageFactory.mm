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
    setSharedObject(obj);
}

MImageImpl::ptr MMACImageFactory::imageFromFFData(const MVector<uint8_t>::ptr &ffData) {
    NSData   *data = [NSData dataWithBytes:ffData->vector.data() length:ffData->vector.size()];
    _NSImage *real = [[_NSImage alloc] initWithData:data];
    
    if (real) {
        auto impl = MMACImageImpl::create();
        impl->mReal = real;
        return impl;
    }
    return nullptr;
}

MImageImpl::ptr MMACImageFactory::imageFromBitmap(const MVector<uint8_t>::ptr &bitmap, int width, int height) {
    auto pixels = MVector<uint8_t>::create(); {
        pixels->vector.resize(width * height * 4);
        
        auto src = (MColorRGBA *)bitmap->vector.data();
        auto dst = (MColorABGR *)pixels->vector.data();
        MColorTransform(src, dst, width * height);
    }
    
    _NSImage *real = nil; {
        CGContextRef context = CreateBitmapContext(pixels->vector.data(), width, height);
        CGImageRef   cgImage = CGBitmapContextCreateImage(context);

    #if TARGET_OS_OSX
        real = [[NSImage alloc] initWithCGImage:cgImage size:NSMakeSize(width, height)];
    #elif TARGET_OS_IOS
        real = [[UIImage alloc] initWithCGImage:cgImage];
    #endif

        CGContextRelease(context);
        CGImageRelease(cgImage);
    }

    auto impl = MMACImageImpl::create();
    impl->mReal = real;
    return impl;
}

MVector<uint8_t>::ptr MMACImageFactory::ffDataFromImage(const MImageImpl::ptr &impl, MImageFileFormat format) {
    _NSImage *real = std::static_pointer_cast<MMACImageImpl>(impl)->mReal;

    NSData *data = nil; {
    #if TARGET_OS_OSX
        NSBitmapImageRep *rep = [NSBitmapImageRep imageRepWithData:real.TIFFRepresentation];
        rep.size = real.size;

        switch (format) {
            case MImageFileFormat::JPEG: data = [rep representationUsingType:NSBitmapImageFileTypeJPEG properties:@{}]; break;
            case MImageFileFormat::PNG : data = [rep representationUsingType:NSBitmapImageFileTypePNG  properties:@{}]; break;
        }
        
    #elif TARGET_OS_IOS
        switch (format) {
            case MImageFileFormat::JPEG: data = UIImageJPEGRepresentation(real, 1); break;
            case MImageFileFormat::PNG : data = UIImagePNGRepresentation (real   ); break;
        }
    #endif
    }

    if (data) {
        auto ffData = MVector<uint8_t>::create();
        ffData->vector.insert(ffData->vector.end(), (uint8_t *)data.bytes, (uint8_t *)data.bytes + data.length);
        return ffData;
    }
    return nil;
}

MVector<uint8_t>::ptr MMACImageFactory::bitmapFromImage(const MImageImpl::ptr &impl) {
    //new bitmap space:
    _NSImage *real = std::static_pointer_cast<MMACImageImpl>(impl)->mReal;
    
    auto width  = (int)real.size.width ;
    auto height = (int)real.size.height;

    auto bitmap = MVector<uint8_t>::create();
    bitmap->vector.resize(width * height * 4);

    //draw image on the bitmap context:
    CGContextRef context = CreateBitmapContext(bitmap->vector.data(), width, height);
    
#if TARGET_OS_OSX
    NSGraphicsContext.currentContext = [NSGraphicsContext graphicsContextWithCGContext:context flipped:NO];
    [real drawInRect:NSMakeRect(0, 0, width, height)];
    NSGraphicsContext.currentContext = nil;
#elif TARGET_OS_IOS
    CGContextDrawImage(context, CGRectMake(0, 0, width, height), real.CGImage);
#endif
    
    CGContextRelease(context);

    //transform color format.
    auto src = (MColorABGR *)bitmap->vector.data();
    auto dst = (MColorRGBA *)bitmap->vector.data();
    MColorTransform(src, dst, width * height);

    return bitmap;
}

MSize::ptr MMACImageFactory::pixelSize(const MImageImpl::ptr &impl) {
    _NSImage *real = std::static_pointer_cast<MMACImageImpl>(impl)->mReal;
    return MSize::from(real.size.width, real.size.height);
}
