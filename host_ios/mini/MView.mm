#import "MView.h"
#import "MIOSAPI.h"
#import "mhostui.h"

@implementation MView

- (void)drawRect:(CGRect)rect {
    _MWindowOnDraw();
    
    int triangleCount = _MWindowTriangleCount();
    for (int index = 0; index < triangleCount; ++index) {
        [self drawTriangle:index];
    }
    
    int imageCount = _MWindowImageCount();
    for (int index = 0; index < imageCount; ++index) {
        [self drawImage:index];
    }
    
    int labelCount = _MWindowLabelCount();
    for (int index = 0; index < labelCount; ++index) {
        [self drawLabel:index];
    }
}

- (void)drawTriangle:(int)index {
    CGContextRef context = UIGraphicsGetCurrentContext();
    
    //set the color:
    MColorPattern color;
    color.rgba = _MWindowTriangleColor(index);
    
    float red   = color.red   / 255.f;
    float green = color.green / 255.f;
    float blue  = color.blue  / 255.f;
    float alpha = color.alpha / 255.f;
    
    CGContextSetRGBFillColor  (context, red, green, blue, alpha);
    CGContextSetRGBStrokeColor(context, red, green, blue, alpha);
    
    //connect the points:
    _MPixel x0 = _MWindowTriangleVertexX(index, 0);
    _MPixel y0 = _MWindowTriangleVertexY(index, 0);
    _MPixel x1 = _MWindowTriangleVertexX(index, 1);
    _MPixel y1 = _MWindowTriangleVertexY(index, 1);
    _MPixel x2 = _MWindowTriangleVertexX(index, 2);
    _MPixel y2 = _MWindowTriangleVertexY(index, 2);
    
    CGContextMoveToPoint   (context, x0, y0);
    CGContextAddLineToPoint(context, x1, y1);
    CGContextAddLineToPoint(context, x2, y2);
    
    //draw.
    CGContextDrawPath(context, kCGPathFillStroke);
}

- (void)drawImage:(int)index {
    //get the image.
    MImage *imageObject  = _MWindowImageObject(index);
    auto    imageLoad    = (MImageLoad *)MImageGetLoad(imageObject);
    UIImage *nativeImage = imageLoad->nativeImage();
    
    //get the position rectangle.
    _MPixel X = _MWindowImageX(index);
    _MPixel Y = _MWindowImageY(index);
    _MPixel W = _MWindowImageWidth (index);
    _MPixel H = _MWindowImageHeight(index);
    
    //draw.
    [nativeImage drawInRect:CGRectMake(X, Y, W, H)];
}

- (void)drawLabel:(int)index {
    //set the color:
    MColorPattern nativeColor;
    nativeColor.rgba = _MWindowLabelColor(index);
    
    float red   = nativeColor.red   / 255.f;
    float green = nativeColor.green / 255.f;
    float blue  = nativeColor.blue  / 255.f;
    float alpha = nativeColor.alpha / 255.f;
    
    UIColor *color = [UIColor colorWithRed:red green:green blue:blue alpha:alpha];
    
    //set the font.
    _MPixel fontSize = _MWindowLabelFontSize(index);
    UIFont *font = [UIFont systemFontOfSize:fontSize];
    
    //calculate the position:
    MString *nativeString = _MWindowLabelString(index);
    NSString *string = @(MStringU8Chars(nativeString));
    
    NSDictionary *attributes = @{
        NSForegroundColorAttributeName: color,
        NSFontAttributeName: font,
    };
    CGSize size = [string sizeWithAttributes:attributes];
    
    _MPixel X = _MWindowLabelX(index);
    _MPixel Y = _MWindowLabelY(index);
    _MPixel W = _MWindowLabelWidth (index);
    _MPixel H = _MWindowLabelHeight(index);
    
    CGPoint origin = CGPointZero;
    MHAlign hAlign = _MWindowLabelHAlign(index);
    MVAlign vAlign = _MWindowLabelVAlign(index);
    if /**/ (hAlign == MHAlign_Left  ) { origin.x = X; }
    else if (hAlign == MHAlign_Center) { origin.x = X + (W - size.width ) / 2; }
    else if (hAlign == MHAlign_Right ) { origin.x = X + (W - size.width ); }
    if /**/ (vAlign == MVAlign_Top   ) { origin.y = Y; }
    else if (vAlign == MVAlign_Center) { origin.y = Y + (H - size.height) / 2; }
    else if (vAlign == MVAlign_Bottom) { origin.y = Y + (H - size.height); }
    
    //draw.
    [string drawAtPoint:origin withAttributes:attributes];
}

@end
