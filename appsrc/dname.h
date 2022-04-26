#pragma once

#define  dash_name      _dash_name_a(__LINE__)
#define _dash_name_a(n) _dash_name_b(n)
#define _dash_name_b(n) _dash_name_##n
