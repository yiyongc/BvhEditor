

#include "tpixelutils.h"

// /*!This method is used to produce current palette colors for cm24 Toonz images.*/
//  static inline TPixelRGBM32 combine(const TPixelRGBM32 &a, const TPixelRGBM32 &b) {
//    return (*(TUINT32*)&a + *(TUINT32*)&b);
// }

//-----------------------------------------------------------------------------

//namespace
//{
//int byteCrop(int v)
//{
//	return (unsigned int)v <= 255 ? v : v > 255 ? 255 : 0;
//}
//int wordCrop(int v)
//{
//	return (unsigned int)v <= 65535 ? v : v > 65535 ? 65535 : 0;
//}
//} // namespace
//
////-----------------------------------------------------------------------------
//
//void hsv2rgb(TPixel32 &dstRgb, int srcHsv[3], int maxHsv)
//{
//	int i;
//	double p, q, t, f;
//	double hue, sat, value;
//	assert(maxHsv);
//	hue = ((double)srcHsv[0] / maxHsv) * 360.;
//	sat = (double)srcHsv[1] / maxHsv;
//	value = (double)srcHsv[2] / maxHsv;
//
//	if (hue > 360)
//		hue -= 360;
//	if (hue < 0)
//		hue += 360;
//	if (sat < 0)
//		sat = 0;
//	if (sat > 1)
//		sat = 1;
//	if (value < 0)
//		value = 0;
//	if (value > 1)
//		value = 1;
//	if (sat == 0) {
//		dstRgb.r = dstRgb.g = dstRgb.b = tcrop((int)(value * 255), 0, 255);
//	} else {
//		if (hue == 360)
//			hue = 0;
//
//		hue = hue / 60;
//		i = (int)hue;
//		f = hue - i;
//		p = value * (1 - sat);
//		q = value * (1 - (sat * f));
//		t = value * (1 - (sat * (1 - f)));
//
//		if (i == 0) {
//			dstRgb.r = tcrop((int)(value * 255), 0, 255);
//			dstRgb.g = tcrop((int)(t * 255), 0, 255);
//			dstRgb.b = tcrop((int)(p * 255), 0, 255);
//		} else if (i == 1) {
//
//			dstRgb.r = tcrop((int)(q * 255), 0, 255);
//			dstRgb.g = tcrop((int)(value * 255), 0, 255);
//			dstRgb.b = tcrop((int)(p * 255), 0, 255);
//		} else if (i == 2) {
//			dstRgb.r = tcrop((int)(p * 255), 0, 255);
//			dstRgb.g = tcrop((int)(value * 255), 0, 255);
//			dstRgb.b = tcrop((int)(t * 255), 0, 255);
//		} else if (i == 3) {
//			dstRgb.r = tcrop((int)(p * 255), 0, 255);
//			dstRgb.g = tcrop((int)(q * 255), 0, 255);
//			dstRgb.b = tcrop((int)(value * 255), 0, 255);
//		} else if (i == 4) {
//			dstRgb.r = tcrop((int)(t * 255), 0, 255);
//			dstRgb.g = tcrop((int)(p * 255), 0, 255);
//			dstRgb.b = tcrop((int)(value * 255), 0, 255);
//		} else if (i == 5) {
//			dstRgb.r = tcrop((int)(value * 255), 0, 255);
//			dstRgb.g = tcrop((int)(p * 255), 0, 255);
//			dstRgb.b = tcrop((int)(q * 255), 0, 255);
//		}
//	}
//	dstRgb.m = 255; //matte
//}
////-----------------------------------------------------------------------------
//
//void HSV2RGB(double hue, double sat, double value,
//			 double *red, double *green, double *blue)
//{
//	int i;
//	double p, q, t, f;
//
//	//  if (hue > 360 || hue < 0)
//	//    hue=0;
//	if (hue > 360)
//		hue -= 360;
//	if (hue < 0)
//		hue += 360;
//	if (sat < 0)
//		sat = 0;
//	if (sat > 1)
//		sat = 1;
//	if (value < 0)
//		value = 0;
//	if (value > 1)
//		value = 1;
//	if (sat == 0) {
//		*red = value;
//		*green = value;
//		*blue = value;
//	} else {
//		if (hue == 360)
//			hue = 0;
//
//		hue = hue / 60;
//		i = (int)hue;
//		f = hue - i;
//		p = value * (1 - sat);
//		q = value * (1 - (sat * f));
//		t = value * (1 - (sat * (1 - f)));
//
//		switch (i) {
//		case 0:
//			*red = value;
//			*green = t;
//			*blue = p;
//			break;
//		case 1:
//			*red = q;
//			*green = value;
//			*blue = p;
//			break;
//		case 2:
//			*red = p;
//			*green = value;
//			*blue = t;
//			break;
//		case 3:
//			*red = p;
//			*green = q;
//			*blue = value;
//			break;
//		case 4:
//			*red = t;
//			*green = p;
//			*blue = value;
//			break;
//		case 5:
//			*red = value;
//			*green = p;
//			*blue = q;
//			break;
//		}
//	}
//}
//
//void RGB2HSV(double r, double g, double b,
//			 double *h, double *s, double *v)
//{
//	double max, min;
//	double delta;
//
//	max = std::max({r, g, b});
//	min = std::min({r, g, b});
//
//	*v = max;
//
//	if (max != 0)
//		*s = (max - min) / max;
//	else
//		*s = 0;
//
//	if (*s == 0)
//		*h = 0;
//	else {
//		delta = max - min;
//
//		if (r == max)
//			*h = (g - b) / delta;
//		else if (g == max)
//			*h = 2 + (b - r) / delta;
//		else if (b == max)
//			*h = 4 + (r - g) / delta;
//		*h = *h * 60;
//		if (*h < 0)
//			*h += 360;
//	}
//}
//void rgb2hsv(int dstHsv[3], const TPixel32 &srcRgb, int maxHsv)
//{
//	double max, min;
//	double delta;
//	double r, g, b;
//	double v, s, h;
//	r = srcRgb.r / 255.;
//	g = srcRgb.g / 255.;
//	b = srcRgb.b / 255.;
//
//	max = std::max({r, g, b});
//	min = std::min({r, g, b});
//
//	v = max;
//
//	if (max != 0)
//		s = (max - min) / max;
//	else
//		s = 0;
//
//	if (s == 0)
//		h = 0;
//	else {
//		delta = max - min;
//
//		if (r == max)
//			h = (g - b) / delta;
//		else if (g == max)
//			h = 2 + (b - r) / delta;
//		else if (b == max)
//			h = 4 + (r - g) / delta;
//		h = h * 60;
//		if (h < 0)
//			h += 360;
//	}
//
//	dstHsv[0] = tcrop((int)((h / 360.) * maxHsv), 0, maxHsv);
//	dstHsv[1] = tcrop((int)(s * maxHsv), 0, maxHsv);
//	dstHsv[2] = tcrop((int)(v * maxHsv), 0, maxHsv);
//}
//
///*!
//  Conversion between RGB and HLS colorspace
//*/
//
//namespace
//{
////helper function
//inline double HLSValue(double n1, double n2, double h)
//{
//	if (h > 360)
//		h -= 360;
//	else if (h < 0)
//		h += 360;
//	if (h < 60)
//		return n1 + (n2 - n1) * h / 60;
//	else if (h < 180)
//		return n2;
//	else if (h < 240)
//		return n1 + (n2 - n1) * (240 - h) / 60;
//	else
//		return n1;
//}
//}
//void HLS2RGB(double h, double l, double s,
//			 double *r, double *g, double *b)
//{
//	if (s == 0) {
//		*r = *g = *b = l;
//		return;
//	}
//
//	double m1, m2;
//
//	if (l < 0.5)
//		m2 = l * (1 + s);
//	else
//		m2 = l + s + l * s;
//	m1 = 2 * l - m2;
//
//	*r = HLSValue(m1, m2, h + 120);
//	*g = HLSValue(m1, m2, h);
//	*b = HLSValue(m1, m2, h - 120);
//}
//
//void rgb2hls(double r, double g, double b,
//			 double *h, double *l, double *s)
//
//{
//	double max, min;
//	double delta;
//
//	max = std::max({r, g, b});
//	min = std::min({r, g, b});
//
//	*l = (max + min) / 2;
//
//	if (max == min) {
//		*s = 0;
//		*h = 0;
//	} else {
//		if (*l <= 0.5)
//			*s = (max - min) / (max + min);
//		else
//			*s = (max - min) / (2 - max - min);
//
//		delta = max - min;
//		if (r == max)
//			*h = (g - b) / delta;
//		else if (g == max)
//			*h = 2 + (b - r) / delta;
//		else if (b == max)
//			*h = 4 + (r - g) / delta;
//
//		*h = *h * 60;
//		if (*h < 0)
//			*h += 360;
//	}
//}

//-----------------------------------------------------------------------------

TPixel32 toPixel32(const TPixel64 &src)
{
	return TPixelRGBM32(
		byteFromUshort(src.r),
		byteFromUshort(src.g),
		byteFromUshort(src.b),
		byteFromUshort(src.m));
}

//-----------------------------------------------------------------------------

//TPixel32 toPixel32(const TPixelD &src)
//{
//	const double factor = 255.0;
//	return TPixel32(
//		byteCrop(tround(src.r * factor)),
//		byteCrop(tround(src.g * factor)),
//		byteCrop(tround(src.b * factor)),
//		byteCrop(tround(src.m * factor)));
//}
//
////-----------------------------------------------------------------------------
//
//TPixel32 toPixel32(const TPixelGR8 &src)
//{
//	return TPixel32(src.value, src.value, src.value);
//}
//
////-----------------------------------------------------------------------------
//
//TPixel64 toPixel64(const TPixel32 &src)
//{
//	return TPixelRGBM64(
//		ushortFromByte(src.r),
//		ushortFromByte(src.g),
//		ushortFromByte(src.b),
//		ushortFromByte(src.m));
//}
//
////-----------------------------------------------------------------------------
//
//TPixel64 toPixel64(const TPixelD &src)
//{
//	const double factor = 65535.0;
//	return TPixel64(
//		wordCrop(tround(src.r * factor)),
//		wordCrop(tround(src.g * factor)),
//		wordCrop(tround(src.b * factor)),
//		wordCrop(tround(src.m * factor)));
//}
//
////-----------------------------------------------------------------------------
//
//TPixel64 toPixel64(const TPixelGR8 &src)
//{
//	int v = ushortFromByte(src.value);
//	return TPixel64(v, v, v);
//}
//
////-----------------------------------------------------------------------------
//
//TPixelD toPixelD(const TPixel32 &src)
//{
//	const double factor = 1.0 / 255.0;
//	return TPixelD(factor * src.r, factor * src.g, factor * src.b, factor * src.m);
//}
//
////-----------------------------------------------------------------------------
//
//TPixelD toPixelD(const TPixel64 &src)
//{
//	const double factor = 1.0 / 65535.0;
//	return TPixelD(factor * src.r, factor * src.g, factor * src.b, factor * src.m);
//}
//
////-----------------------------------------------------------------------------
//
//TPixelD toPixelD(const TPixelGR8 &src)
//{
//	const double v = (double)src.value / 255.0;
//	return TPixelD(v, v, v);
//}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

