"""Reference checks for the fuzzy tables defined in Ball_beam.ino and graficos.py.

Pure standard library: no hardware, no third-party packages, no network.
"""

import unittest


POS = {
    "NM": (0, 0, 3, 6),
    "NS": (3, 6, 9, 12),
    "ZO": (9, 12, 15, 18),
    "PS": (15, 18, 21, 24),
    "PM": (21, 24, 26, 30),
}

SP = {
    "NM": (0, 0, 3, 6),
    "NS": (3, 6, 9, 12),
    "ZO": (9, 12, 15, 18),
    "PS": (15, 18, 21, 24),
    "PM": (21, 24, 27, 30),
}

THETA = {
    "NM": (45, 50, 60, 80),
    "NS": (60, 80, 90, 100),
    "ZO": (90, 100, 110, 120),
    "PS": (110, 120, 120, 130),
    "PM": (130, 140, 160, 180),
}

# (ball position, set point) -> theta output, transcribed from Ball_beam.ino.
RULES = {
    ("NM", "NM"): "ZO", ("NM", "NS"): "NS", ("NM", "ZO"): "NS",
    ("NM", "PS"): "NS", ("NM", "PM"): "NM",
    ("NS", "NM"): "ZO", ("NS", "NS"): "ZO", ("NS", "ZO"): "NS",
    ("NS", "PS"): "NS", ("NS", "PM"): "NM",
    ("ZO", "NM"): "PS", ("ZO", "NS"): "PS", ("ZO", "ZO"): "ZO",
    ("ZO", "PS"): "ZO", ("ZO", "PM"): "NS",
    ("PS", "NM"): "PM", ("PS", "NS"): "PS", ("PS", "ZO"): "ZO",
    ("PS", "PS"): "ZO", ("PS", "PM"): "ZO",
    ("PM", "NM"): "PM", ("PM", "NS"): "PM", ("PM", "ZO"): "PS",
    ("PM", "PS"): "ZO", ("PM", "PM"): "ZO",
}


def trapmf(x, a, b, c, d):
    if x <= a or x >= d:
        return 0.0
    if b <= x <= c:
        return 1.0
    if a < x < b and b > a:
        return (x - a) / (b - a)
    if c < x < d and d > c:
        return (d - x) / (d - c)
    return 0.0


def infer(position, set_point):
    active = []
    for (pos_label, sp_label), theta_label in RULES.items():
        weight = min(trapmf(position, *POS[pos_label]),
                     trapmf(set_point, *SP[sp_label]))
        if weight > 0.0:
            active.append((THETA[theta_label], weight))
    if not active:
        return None
    numerator = denominator = 0.0
    for x in range(45, 181):
        mu = max(min(trapmf(x, *params), weight) for params, weight in active)
        numerator += x * mu
        denominator += mu
    return numerator / denominator


class FuzzyTablesTest(unittest.TestCase):

    def test_rule_count_matches_firmware(self):
        self.assertEqual(len(RULES), 25)

    def test_every_label_combination_is_defined(self):
        self.assertEqual(set(RULES), {(p, s) for p in POS for s in SP})

    def test_rule_outputs_are_known_labels(self):
        self.assertTrue(set(RULES.values()) <= set(THETA))

    def test_membership_cores_and_boundaries(self):
        for table in (POS, SP, THETA):
            for params in table.values():
                a, b, c, d = params
                self.assertAlmostEqual(trapmf(c, *params), 1.0)
                self.assertEqual(trapmf(a, *params), 0.0)
                self.assertEqual(trapmf(d, *params), 0.0)

    def test_every_rule_can_fire(self):
        for (pos_label, sp_label) in RULES:
            pos_core = (POS[pos_label][1] + POS[pos_label][2]) / 2
            sp_core = (SP[sp_label][1] + SP[sp_label][2]) / 2
            self.assertGreater(trapmf(pos_core, *POS[pos_label]), 0.9)
            self.assertGreater(trapmf(sp_core, *SP[sp_label]), 0.9)
            self.assertIsNotNone(infer(pos_core, sp_core))

    def test_inference_is_deterministic_and_bounded(self):
        first = infer(13.5, 13.5)
        second = infer(13.5, 13.5)
        self.assertEqual(first, second)
        self.assertGreaterEqual(first, 45.0)
        self.assertLessEqual(first, 180.0)
        self.assertGreaterEqual(first, 95.0)
        self.assertLessEqual(first, 115.0)


if __name__ == "__main__":
    unittest.main()
