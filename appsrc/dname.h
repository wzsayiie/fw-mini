#pragma once

#define  d_name      _d_name_a(__LINE__)
#define _d_name_a(n) _d_name_b(n)
#define _d_name_b(n) _d_name_##n
