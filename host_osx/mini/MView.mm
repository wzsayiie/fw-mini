#import "MView.h"
#import "MOSXAPI.h"
#import "mhostui.h"

@implementation MView

- (void)drawRect:(NSRect)dirtyRect {
    _MWindowOnDraw();
    
    float height = self.frame.size.height;

    int count = _MWindowGraphCount();
    for (int index = 0; index < count; ++index) {
        _MGraph graph = _MWindowGraphType(index);
        
        switch (graph) {
            case _MGraph_Triangle: [self drawTriangle:index withViewHeight:height]; break;
            case _MGraph_Image   : [self drawImage   :index withViewHeight:height]; break;
            case _MGraph_Label   : [self drawLabel   :index withViewHeight:height]; break;
            default:;
        }
    }
}

- (void)drawTriangle:(int)index withViewHeight:(float)viewHeight {
    CGContextRef context = NSGraphicsContext.currentContext.CGContext;
    
    //set the color:
    MColorPattern color;
    color.color = _MWindowTriangleGraphColor(index);
    
    float red   = color.red   / 255.f;
    float green = color.green / 255.f;
    float blue  = color.blue  / 255.f;
    float alpha = color.alpha / 255.f;
    
    CGContextSetRGBFillColor  (context, red, green, blue, alpha);
    CGContextSetRGBStrokeColor(context, red, green, blue, alpha);
    
    //connect the points:
    _MPixel x0 = _MWindowTriangleGraphX0(index);
    _MPixel y0 = _MWindowTriangleGraphY0(index);
    _MPixel x1 = _MWindowTriangleGraphX1(index);
    _MPixel y1 = _MWindowTriangleGraphY1(index);
    _MPixel x2 = _MWindowTriangleGraphX2(index);
    _MPixel y2 = _MWindowTriangleGraphY2(index);
    
    CGContextMoveToPoint   (context, x0, viewHeight - y0);
    CGContextAddLineToPoint(context, x1, viewHeight - y1);
    CGContextAddLineToPoint(context, x2, viewHeight - y2);
    
    //draw.
    CGContextDrawPath(context, kCGPathFillStroke);
}

- (void)drawImage:(int)index withViewHeight:(float)viewHeight {
    //get the image.
    auto imageObject = (MOSXImage *)_MWindowImageGraphObject(index);
    NSImage *nativeImage = imageObject->nativeImage();
    
    //get the position rectangle.
    _MPixel W = _MWindowImageGraphWidth (index);
    _MPixel H = _MWindowImageGraphHeight(index);
    _MPixel X = _MWindowImageGraphX(index);
    _MPixel Y = viewHeight - H - _MWindowImageGraphY(index);
    
    //draw.
    [nativeImage drawInRect:NSMakeRect(X, Y, W, H)];
}

- (void)drawLabel:(int)index withViewHeight:(float)viewHeight {
    //set the color:
    MColorPattern nativeColor;
    nativeColor.color = _MWindowLabelGraphColor(index);
    
    float red   = nativeColor.red   / 255.f;
    float green = nativeColor.green / 255.f;
    float blue  = nativeColor.blue  / 255.f;
    float alpha = nativeColor.alpha / 255.f;
    
    NSColor *color = [NSColor colorWithRed:red green:green blue:blue alpha:alpha];
    
    //set the font.
    _MPixel fontSize = _MWindowLabelGraphFontSize(index);
    NSFont *font = [NSFont systemFontOfSize:fontSize];
    
    //calculate the position:
    MString *nativeString = _MWindowLabelGraphString(index);
    NSString *string = @(MStringU8Chars(nativeString));
    
    NSDictionary *attributes = @{
        NSForegroundColorAttributeName: color,
        NSFontAttributeName: font,
    };
    NSSize size = [string sizeWithAttributes:attributes];
    
    _MPixel W = _MWindowLabelGraphWidth (index);
    _MPixel H = _MWindowLabelGraphHeight(index);
    _MPixel X = _MWindowLabelGraphX(index);
    _MPixel Y = viewHeight - H - _MWindowLabelGraphY(index);
    
    NSPoint origin = NSZeroPoint;
    MHAlign hAligns = _MWindowLabelGraphHAlign(index);
    MVAlign vAligns = _MWindowLabelGraphVAlign(index);
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
