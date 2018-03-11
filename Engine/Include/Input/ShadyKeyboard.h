#pragma once

namespace Shady {
	namespace Input {
		enum KeyboardKeys {
			a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z,
			one, two, three, four, five, six, seven, eight, nine, zero, left, right, up, down
		};

		enum KeyboardKeyState {
			pressed,
			held,
			released
		};
		class ShadyKeyboard {
		public:
			ShadyKeyboard();
			~ShadyKeyboard();
		};
	}
}
