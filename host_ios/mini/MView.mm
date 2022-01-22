#import "MView.h"
#import "MIOSAPI.h"
#import "mhostui.h"

@implementation MView

- (instancetype)initWithFrame:(CGRect)frame {
    if (self = [super initWithFrame:frame]) {
        //by default, "UIView" will not clear the buffer before redrawing.
        self.backgroundColor = UIColor.whiteColor;
    }
    return self;
}

- (void)drawRect:(CGRect)rect {
    _MWindowOnDraw();
    
    int count = _MWindowGraphCount();
    for (int index = 0; index < count; ++index) {
        _MGraph graph = _MWindowGraphType(index);
        
        switch (graph) {
            case _MGraph_Triangle: [self drawTriangle:index]; break;
            case _MGraph_Image   : [self drawImage   :index]; break;
            case _MGraph_Label   : [self drawLabel   :index]; break;
        }
    }
}

- (void)drawTriangle:(int)index {
    CGContextRef context = UIGraphicsGetCurrentContext();
    
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
    
    CGContextMoveToPoint   (context, x0, y0);
    CGContextAddLineToPoint(context, x1, y1);
    CGContextAddLineToPoint(context, x2, y2);
    
    //draw.
    CGContextDrawPath(context, kCGPathFillStroke);
}

- (void)drawImage:(int)index {
    //get the image.
    auto imageObject = (MIOSImage *)_MWindowImageGraphObject(index);
    UIImage *uiImage = imageObject->uiImage();
    
    //get the position rectangle.
    _MPixel X = _MWindowImageGraphX     (index);
    _MPixel Y = _MWindowImageGraphY     (index);
    _MPixel W = _MWindowImageGraphWidth (index);
    _MPixel H = _MWindowImageGraphHeight(index);
    
    //draw.
    [uiImage drawInRect:CGRectMake(X, Y, W, H)];
}

- (void)drawLabel:(int)index {
    //set the color:
    MColorPattern nativeColor;
    nativeColor.color = _MWindowLabelGraphColor(index);
    
    float red   = nativeColor.red   / 255.f;
    float green = nativeColor.green / 255.f;
    float blue  = nativeColor.blue  / 255.f;
    float alpha = nativeColor.alpha / 255.f;
    
    UIColor *color = [UIColor colorWithRed:red green:green blue:blue alpha:alpha];
    
    //set the font.
    _MPixel fontSize = _MWindowLabelGraphFontSize(index);
    UIFont *font = [UIFont systemFontOfSize:fontSize];
    
    //calculate the position:
    MString *nativeString = _MWindowLabelGraphString(index);
    NSString *string = @(MStringU8Chars(nativeString));
    
    NSDictionary *attributes = @{
        NSForegroundColorAttributeName: color,
        NSFontAttributeName: font,
    };
    CGSize size = [string sizeWithAttributes:attributes];
    
    _MPixel X = _MWindowLabelGraphX     (index);
    _MPixel Y = _MWindowLabelGraphY     (index);
    _MPixel W = _MWindowLabelGraphWidth (index);
    _MPixel H = _MWindowLabelGraphHeight(index);
    
    CGPoint origin = CGPointZero;
    MHAlign hAlign = _MWindowLabelGraphHAlign(index);
    MVAlign vAlign = _MWindowLabelGraphVAlign(index);
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
