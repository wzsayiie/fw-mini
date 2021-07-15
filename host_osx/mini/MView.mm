#import "MView.h"
#import "mhostui.h"

@implementation MView

- (void)drawRect:(NSRect)dirtyRect {
    _MWindowOnDraw();
    
    float height = self.frame.size.height;

    int triangleCount = _MWindowTriangleCount();
    for (int index = 0; index < triangleCount; ++index) {
        [self drawTriangle:index withViewHeight:height];
    }

    int labelCount = _MWindowLabelCount();
    for (int index = 0; index < labelCount; ++index) {
        [self drawLabel:index withViewHeight:height];
    }
    
    int imageCount = _MWindowImageCount();
    for (int index = 0; index < imageCount; ++index) {
        [self drawImage:index withViewHeight:height];
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
    float x0 = _MWindowTriangleVertexX(index, 0);
    float y0 = _MWindowTriangleVertexY(index, 0);
    float x1 = _MWindowTriangleVertexX(index, 1);
    float y1 = _MWindowTriangleVertexY(index, 1);
    float x2 = _MWindowTriangleVertexX(index, 2);
    float y2 = _MWindowTriangleVertexY(index, 2);
    
    CGContextMoveToPoint   (context, x0, viewHeight - y0);
    CGContextAddLineToPoint(context, x1, viewHeight - y1);
    CGContextAddLineToPoint(context, x2, viewHeight - y2);
    
    //draw.
    CGContextDrawPath(context, kCGPathFillStroke);
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
    float fontSize = _MWindowLabelFontSize(index);
    NSFont *font = [NSFont systemFontOfSize:fontSize];
    
    //calculate the position:
    MString *nativeString = _MWindowLabelString(index);
    NSString *string = @(MStringU8Chars(nativeString));
    
    NSDictionary *attributes = @{
        NSForegroundColorAttributeName: color,
        NSFontAttributeName: font,
    };
    NSSize size = [string sizeWithAttributes:attributes];
    
    float W = _MWindowLabelWidth (index);
    float H = _MWindowLabelHeight(index);
    float X = _MWindowLabelX(index);
    float Y = viewHeight - H - _MWindowLabelY(index);
    
    NSPoint origin = NSZeroPoint;
    MAligns aligns = _MWindowLabelAligns(index);
    if /**/ (aligns & MAlign_Left   ) { origin.x = X; }
    else if (aligns & MAlign_HCenter) { origin.x = X + (W - size.width ) / 2; }
    else if (aligns & MAlign_Right  ) { origin.x = X + (W - size.width ); }
    if /**/ (aligns & MAlign_Top    ) { origin.y = Y + (H - size.height); }
    else if (aligns & MAlign_VCenter) { origin.y = Y + (H - size.height) / 2; }
    else if (aligns & MAlign_Bottom ) { origin.y = Y; }
    
    //draw.
    [string drawAtPoint:origin withAttributes:attributes];
}

- (void)drawImage:(int)index withViewHeight:(float)viewHeight {
}

@end
