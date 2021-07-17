#import "MView.h"
#import "MOSXAPI.h"
#import "mhostui.h"

@implementation MView

- (void)drawRect:(NSRect)dirtyRect {
    _MWindowOnDraw();
    
    float height = self.frame.size.height;

    int triangleCount = _MWindowTriangleCount();
    for (int index = 0; index < triangleCount; ++index) {
        [self drawTriangle:index withViewHeight:height];
    }

    int imageCount = _MWindowImageCount();
    for (int index = 0; index < imageCount; ++index) {
        [self drawImage:index withViewHeight:height];
    }
    
    int labelCount = _MWindowLabelCount();
    for (int index = 0; index < labelCount; ++index) {
        [self drawLabel:index withViewHeight:height];
    }
}

- (void)drawTriangle:(int)index withViewHeight:(float)viewHeight {
    CGContextRef context = NSGraphicsContext.currentContext.CGContext;
    
    //set the color:
    MColorPattern color = {
        .rgba = _MWindowTriangleColor(index)
    };
    
    float red   = color.red   / 255.f;
    float green = color.green / 255.f;
    float blue  = color.blue  / 255.f;
    float alpha = color.alpha / 255.f;
    
    CGContextSetRGBFillColor  (context, red, green, blue, alpha);
    CGContextSetRGBStrokeColor(context, red, green, blue, alpha);
    
    //connect the points:
    _WPIXEL x0 = _MWindowTriangleVertexX(index, 0);
    _WPIXEL y0 = _MWindowTriangleVertexY(index, 0);
    _WPIXEL x1 = _MWindowTriangleVertexX(index, 1);
    _WPIXEL y1 = _MWindowTriangleVertexY(index, 1);
    _WPIXEL x2 = _MWindowTriangleVertexX(index, 2);
    _WPIXEL y2 = _MWindowTriangleVertexY(index, 2);
    
    CGContextMoveToPoint   (context, x0, viewHeight - y0);
    CGContextAddLineToPoint(context, x1, viewHeight - y1);
    CGContextAddLineToPoint(context, x2, viewHeight - y2);
    
    //draw.
    CGContextDrawPath(context, kCGPathFillStroke);
}

- (void)drawImage:(int)index withViewHeight:(float)viewHeight {
    //get the image.
    MImage *image = _MWindowImageObject(index);
    NSImage *managedImage = MManagedImage(MImageManagedId(image));
    
    //get the position rectangle.
    _WPIXEL W = _MWindowImageWidth (index);
    _WPIXEL H = _MWindowImageHeight(index);
    _WPIXEL X = _MWindowImageX(index);
    _WPIXEL Y = viewHeight - H - _MWindowImageY(index);
    
    //draw.
    [managedImage drawInRect:NSMakeRect(X, Y, W, H)];
}

- (void)drawLabel:(int)index withViewHeight:(float)viewHeight {
    //set the color:
    MColorPattern nativeColor = {
        .rgba = _MWindowLabelColor(index)
    };
    
    float red   = nativeColor.red   / 255.f;
    float green = nativeColor.green / 255.f;
    float blue  = nativeColor.blue  / 255.f;
    float alpha = nativeColor.alpha / 255.f;
    
    NSColor *color = [NSColor colorWithRed:red green:green blue:blue alpha:alpha];
    
    //set the font.
    _WPIXEL fontSize = _MWindowLabelFontSize(index);
    NSFont *font = [NSFont systemFontOfSize:fontSize];
    
    //calculate the position:
    MString *nativeString = _MWindowLabelString(index);
    NSString *string = @(MStringU8Chars(nativeString));
    
    NSDictionary *attributes = @{
        NSForegroundColorAttributeName: color,
        NSFontAttributeName: font,
    };
    NSSize size = [string sizeWithAttributes:attributes];
    
    _WPIXEL W = _MWindowLabelWidth (index);
    _WPIXEL H = _MWindowLabelHeight(index);
    _WPIXEL X = _MWindowLabelX(index);
    _WPIXEL Y = viewHeight - H - _MWindowLabelY(index);
    
    NSPoint origin = NSZeroPoint;
    MHAlign hAligns = _MWindowLabelHAlign(index);
    MVAlign vAligns = _MWindowLabelVAlign(index);
    if /**/ (hAligns == MHAlign_Left  ) { origin.x = X; }
    else if (hAligns == MHAlign_Center) { origin.x = X + (W - size.width ) / 2; }
    else if (hAligns == MHAlign_Right ) { origin.x = X + (W - size.width ); }
    if /**/ (vAligns == MVAlign_Top   ) { origin.y = Y + (H - size.height); }
    else if (vAligns == MVAlign_Center) { origin.y = Y + (H - size.height) / 2; }
    else if (vAligns == MVAlign_Bottom) { origin.y = Y; }
    
    //draw.
    [string drawAtPoint:origin withAttributes:attributes];
}

@end
