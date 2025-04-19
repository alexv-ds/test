#include "hsv_rgb_conversion.hpp"
#include <limits>

namespace engine {

  // https://stackoverflow.com/questions/3018313/algorithm-to-convert-rgb-to-hsv-and-hsv-to-rgb-in-range-0-255-for-both

  Rgb hsv2rgb(Hsv hsv) noexcept {
    double hh, p, q, t, ff;
    long i;
    Rgb out;

    if (hsv.s <= 0.0) { // < is bogus, just shuts up warnings
      out.r = hsv.v;
      out.g = hsv.v;
      out.b = hsv.v;
      return out;
    }
    hh = hsv.h;
    if (hh >= 360.0)
      hh = 0.0;
    hh /= 60.0;
    i = (long)hh;
    ff = hh - i;
    p = hsv.v * (1.0 - hsv.s);
    q = hsv.v * (1.0 - (hsv.s * ff));
    t = hsv.v * (1.0 - (hsv.s * (1.0 - ff)));

    switch (i) {
    case 0:
      out.r = hsv.v;
      out.g = t;
      out.b = p;
      break;
    case 1:
      out.r = q;
      out.g = hsv.v;
      out.b = p;
      break;
    case 2:
      out.r = p;
      out.g = hsv.v;
      out.b = t;
      break;

    case 3:
      out.r = p;
      out.g = q;
      out.b = hsv.v;
      break;
    case 4:
      out.r = t;
      out.g = p;
      out.b = hsv.v;
      break;
    case 5:
    default:
      out.r = hsv.v;
      out.g = p;
      out.b = q;
      break;
    }
    return out;
  }

  Hsv rgb2hsv(Rgb rgb) noexcept {
    Hsv out;
    double min, max, delta;

    min = rgb.r < rgb.g ? rgb.r : rgb.g;
    min = min < rgb.b ? min : rgb.b;

    max = rgb.r > rgb.g ? rgb.r : rgb.g;
    max = max > rgb.b ? max : rgb.b;

    out.v = max; // v
    delta = max - min;
    if (delta < 0.00001) {
      out.s = 0;
      out.h = 0; // undefined, maybe nan?
      return out;
    }
    if (max > 0.0) { // NOTE: if Max is == 0, this divide would cause a crash
      out.s = (delta / max); // s
    }
    else {
      // if max is 0, then r = g = b = 0
      // s = 0, h is undefined
      out.s = 0.0;
      out.h = std::numeric_limits<float>::quiet_NaN(); // its now undefined
      return out;
    }
    if (rgb.r >= max) // > is bogus, just keeps compilor happy
      out.h = (rgb.g - rgb.b) / delta; // between yellow & magenta
    else if (rgb.g >= max)
      out.h = 2.0 + (rgb.b - rgb.r) / delta; // between cyan & yellow
    else
      out.h = 4.0 + (rgb.r - rgb.g) / delta; // between magenta & cyan

    out.h *= 60.0; // degrees

    if (out.h < 0.0)
      out.h += 360.0;

    return out;
  }
} // namespace engine
