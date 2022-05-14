#import "MMACDrawView.h"
#import "MMACImageFactory.h"
#import "mcontext.h"
#import "mwindow.h"

@implementation MMACDrawView

- (instancetype)initWithFrame:(CGRect)frame {
    if (self = [super initWithFrame:frame]) {
    #if TARGET_OS_IOS
        //by default, "UIView" will not clear the buffer before redrawing.
        self.backgroundColor = UIColor.whiteColor;
    #endif
    }
    return self;
}

- (void)drawRect:(CGRect)dirtyRect {
    MWindow::mainWindow()->draw();
    
#if TARGET_OS_OSX
    CGContextRef context = NSGraphicsContext.currentContext.CGContext;
#elif TARGET_OS_IOS
    CGContextRef context = UIGraphicsGetCurrentContext();
#endif
    CGFloat viewHeight = self.frame.size.height;
    
    MVector<MGraph::ptr> *graphs = MContextGetGraphs();
    for (auto &graph : *graphs) {
        switch (graph->type()) {
            case MGraphType::Clip    : [self drawClip    :(MClipGraph     *)graph.get() withContext: context viewHeight:viewHeight]; break;
            case MGraphType::Triangle: [self drawTriangle:(MTriangleGraph *)graph.get() withContext: context viewHeight:viewHeight]; break;
            case MGraphType::Image   : [self drawImage   :(MImageGraph    *)graph.get() withContext: context viewHeight:viewHeight]; break;
            case MGraphType::Text    : [self drawText    :(MTextGraph     *)graph.get() withContext: context viewHeight:viewHeight]; break;
        }
    }
    
    MContextReset();
}

- (void)drawClip:(MClipGraph *)graph withContext:(CGContextRef)context viewHeight:(CGFloat)viewHeight {
    CGFloat w = graph->pixelW();
    CGFloat h = graph->pixelH();
    CGFloat x = graph->pixelX();
#if TARGET_OS_OSX
    //on mac os, the origin of a rectangle is the lower left corner.
    CGFloat y = viewHeight - h - graph->pixelY();
#elif TARGET_OS_IOS
    CGFloat y = graph->pixelY();
#endif
    
    CGContextResetClip(context);
    CGContextClipToRect(context, CGRectMake(x, y, w, h));
}

- (void)drawTriangle:(MTriangleGraph *)graph withContext:(CGContextRef)context viewHeight:(CGFloat)viewHeight {
    //set the color:
    MColorRGBA rgba = { graph->rgba() };
        
    CGFloat red   = rgba.red   / 255.f;
    CGFloat green = rgba.green / 255.f;
    CGFloat blue  = rgba.blue  / 255.f;
    CGFloat alpha = rgba.alpha / 255.f;
        
    CGContextSetRGBFillColor  (context, red, green, blue, alpha);
    CGContextSetRGBStrokeColor(context, red, green, blue, alpha);
    
    //connect the vertices:
    CGFloat x0 = graph->pixelX0();
    CGFloat x1 = graph->pixelX1();
    CGFloat x2 = graph->pixelX2();
#if TARGET_OS_OSX
    CGFloat y0 = viewHeight - graph->pixelY0();
    CGFloat y1 = viewHeight - graph->pixelY1();
    CGFloat y2 = viewHeight - graph->pixelY2();
#elif TARGET_OS_IOS
    CGFloat y0 = graph->pixelY0();
    CGFloat y1 = graph->pixelY1();
    CGFloat y2 = graph->pixelY2();
#endif
    
    CGContextMoveToPoint   (context, x0, y0);
    CGContextAddLineToPoint(context, x1, y1);
    CGContextAddLineToPoint(context, x2, y2);
        
    //draw.
    CGContextDrawPath(context, kCGPathFillStroke);
}

- (void)drawImage:(MImageGraph *)graph withContext:(CGContextRef)context viewHeight:(CGFloat)viewHeight {
    _NSImage *real = std::static_pointer_cast<MMACImageImpl>(graph->image()->impl())->mReal;
    
    CGFloat w = graph->pixelW();
    CGFloat h = graph->pixelH();
    CGFloat x = graph->pixelX();
#if TARGET_OS_OSX
    CGFloat y = viewHeight - h - graph->pixelY();
#elif TARGET_OS_IOS
    CGFloat y = graph->pixelY();
#endif
    
    [real drawInRect:CGRectMake(x, y, w, h)];
}

- (void)drawText:(MTextGraph *)graph withContext:(CGContextRef)context viewHeight:(CGFloat)viewHeight {
    //set the text.
    NSString *text = @(graph->text().c_str());
    
    //set the color:
    MColorRGBA rgba = { graph->rgba() };
    
    CGFloat red   = rgba.red   / 255.f;
    CGFloat green = rgba.green / 255.f;
    CGFloat blue  = rgba.blue  / 255.f;
    CGFloat alpha = rgba.alpha / 255.f;
    
    _NSColor *color = [_NSColor colorWithRed:red green:green blue:blue alpha:alpha];
    
    //set the font.
    CGFloat fontSize = graph->pixelFontSize();
    _NSFont *font = [_NSFont systemFontOfSize:fontSize];
    
    //calculate the position:
    CGFloat spaceW = graph->pixelW();
    CGFloat spaceH = graph->pixelH();
    CGFloat spaceX = graph->pixelX();
#if TARGET_OS_OSX
    CGFloat spaceY = viewHeight - spaceH - graph->pixelY();
#elif TARGET_OS_IOS
    CGFloat spaceY = graph->pixelY();
#endif
    
    NSDictionary *attributes = @{
        NSForegroundColorAttributeName: color,
        NSFontAttributeName: font,
    };
    CGSize size = [text sizeWithAttributes:attributes];
        
    CGFloat x = 0;
    switch (graph->hAlign()) {
        case MHAlign::Left  : x = spaceX                             ; break;
        case MHAlign::Center: x = spaceX + (spaceW - size.width ) / 2; break;
        case MHAlign::Right : x = spaceX + (spaceW - size.width )    ; break;
    }
    
    CGFloat y = 0;
    switch (graph->vAlign()) {
    #if TARGET_OS_OSX
        case MVAlign::Top   : y = spaceY + (spaceH - size.height)    ; break;
        case MVAlign::Middle: y = spaceY + (spaceH - size.height) / 2; break;
        case MVAlign::Bottom: y = spaceY                             ; break;
    #elif TARGET_OS_IOS
        case MVAlign::Top   : y = spaceY                             ; break;
        case MVAlign::Middle: y = spaceY + (spaceH - size.height) / 2; break;
        case MVAlign::Bottom: y = spaceY + (spaceH - size.height)    ; break;
    #endif
    }
    
    //draw.
    [text drawAtPoint:CGPointMake(x, y) withAttributes:attributes];
}

@end
