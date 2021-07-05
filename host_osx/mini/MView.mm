#import "MView.h"
#import "mhostui.h"

@implementation MView

- (void)drawRect:(NSRect)dirtyRect {
    _MWindowOnDraw();
    
    CGContextRef context = NSGraphicsContext.currentContext.CGContext;
    float height = self.frame.size.height;

    int triangleCount = _MWindowTriangleCount();
    for (int index = 0; index < triangleCount; ++index) {
        [self context:context drawTriangle:index viewHeight:height];
    }

    int labelCount = _MWindowLabelCount();
    for (int index = 0; index < labelCount; ++index) {
        [self context:context drawLabel:index viewHeight:height];
    }
}

- (void)context:(CGContextRef)context setFillStrokeRGBA:(MColor)rgba {
    auto color = (MColorPattern *)&rgba;
    
    float red   = color->red   / 255.f;
    float green = color->green / 255.f;
    float blue  = color->blue  / 255.f;
    float alpha = color->alpha / 255.f;
    
    CGContextSetRGBFillColor  (context, red, green, blue, alpha);
    CGContextSetRGBStrokeColor(context, red, green, blue, alpha);
}

- (void)context:(CGContextRef)context drawTriangle:(int)index viewHeight:(float)height {
    MColor rgba = _MWindowTriangleColor(index);
    [self context:context setFillStrokeRGBA:rgba];
    CGContextSetLineWidth(context, 1);
    
    float x0 = _MWindowTriangleVertexX(index, 0);
    float y0 = _MWindowTriangleVertexY(index, 0);
    float x1 = _MWindowTriangleVertexX(index, 1);
    float y1 = _MWindowTriangleVertexY(index, 1);
    float x2 = _MWindowTriangleVertexX(index, 2);
    float y2 = _MWindowTriangleVertexY(index, 2);
    
    CGContextMoveToPoint   (context, x0, height - y0);
    CGContextAddLineToPoint(context, x1, height - y1);
    CGContextAddLineToPoint(context, x2, height - y2);
    
    CGContextDrawPath(context, kCGPathFillStroke);
}

- (void)context:(CGContextRef)context drawLabel:(int)index viewHeight:(float)height {
}

@end
