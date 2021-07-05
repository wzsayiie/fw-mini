#import "MView.h"
#import "mhostui.h"

@implementation MView

- (void)drawRect:(CGRect)rect {
    _MWindowOnDraw();
    
    CGContextRef context = UIGraphicsGetCurrentContext();
    
    int triangleCount = _MWindowTriangleCount();
    for (int index = 0; index < triangleCount; ++index) {
        [self context:context drawTriangle:index];
    }
    
    int labelCount = _MWindowLabelCount();
    for (int index = 0; index < labelCount; ++index) {
        [self context:context drawLabel:index];
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

- (void)context:(CGContextRef)context drawTriangle:(int)index {
    MColor rgba = _MWindowTriangleColor(index);
    [self context:context setFillStrokeRGBA:rgba];
    CGContextSetLineWidth(context, 1);
    
    float x0 = _MWindowTriangleVertexX(index, 0);
    float y0 = _MWindowTriangleVertexY(index, 0);
    float x1 = _MWindowTriangleVertexX(index, 1);
    float y1 = _MWindowTriangleVertexY(index, 1);
    float x2 = _MWindowTriangleVertexX(index, 2);
    float y2 = _MWindowTriangleVertexY(index, 2);
    
    CGContextMoveToPoint   (context, x0, y0);
    CGContextAddLineToPoint(context, x1, y1);
    CGContextAddLineToPoint(context, x2, y2);
    
    CGContextDrawPath(context, kCGPathFillStroke);
}

- (void)context:(CGContextRef)context drawLabel:(int)index {
}

@end
