function I = J_Empirical(sigma, q, CountSamples)

Samples = GaussianSamples(sigma, q, CountSamples);
I = Expected_log(Samples);