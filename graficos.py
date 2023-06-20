
# CAMILO COVARRUBIAS HERNÁNDEZ 

import numpy as np
import skfuzzy as fuzz
import matplotlib.pyplot as plt

# Variables universales
x_ps = np.arange(0,30,1)
x_theta = np.arange(50,170,10)

ps_nm = fuzz.trapmf(x_ps,[0,0,3,6])
ps_ns = fuzz.trapmf(x_ps,[3,6,9,12])
ps_zo = fuzz.trapmf(x_ps,[9,12,15,18])
ps_ps = fuzz.trapmf(x_ps,[15,18,21,24])
ps_pm = fuzz.trapmf(x_ps,[21,24,26,30])

theta_nm = fuzz.trapmf(x_theta,[50,50,60,80])
theta_ns = fuzz.trapmf(x_theta,[60,80,90,100])
theta_zo = fuzz.trapmf(x_theta,[90,100,110,120])
theta_ps = fuzz.trapmf(x_theta,[110,120,120,130])
theta_pm = fuzz.trapmf(x_theta,[130,140,160,170])

# Visualizacion

fig, (ax0,ax1,ax2) = plt.subplots(nrows=3, figsize=(8,9))

ax0.plot(x_ps, ps_nm, 'r', linewidth=1.5, label='Negative Medium')
ax0.plot(x_ps, ps_ns, 'y', linewidth=1.5, label='Negative Small')
ax0.plot(x_ps, ps_zo, 'g', linewidth=1.5, label='Zero')
ax0.plot(x_ps, ps_ps, 'c', linewidth=1.5, label='Positive Small')
ax0.plot(x_ps, ps_pm, 'b', linewidth=1.5, label='Positive Medium')
ax0.set_title('Ball Position')
ax0.legend()

ax2.plot(x_theta, theta_nm, 'r', linewidth=1.5, label='Negative Medium')
ax2.plot(x_theta, theta_ns, 'y', linewidth=1.5, label='Negative Small')
ax2.plot(x_theta, theta_zo, 'g', linewidth=1.5, label='Zero')
ax2.plot(x_theta, theta_ps, 'c', linewidth=1.5, label='Positive Small')
ax2.plot(x_theta, theta_pm, 'b', linewidth=1.5, label='Positive Medium')
ax2.set_title('Theta Output')
ax2.legend()

ax1.plot(x_ps, ps_nm, 'r', linewidth=1.5, label='Negative Medium')
ax1.plot(x_ps, ps_ns, 'y', linewidth=1.5, label='Negative Small')
ax1.plot(x_ps, ps_zo, 'g', linewidth=1.5, label='Zero')
ax1.plot(x_ps, ps_ps, 'c', linewidth=1.5, label='Positive Small')
ax1.plot(x_ps, ps_pm, 'b', linewidth=1.5, label='Positive Medium')
ax1.set_title('Set Point Position')
ax1.legend()

for ax in (ax0, ax1, ax2):
  ax.spines['top'].set_visible(False)
  ax.spines['right'].set_visible(False)
  ax.get_xaxis().tick_bottom()
  ax.get_yaxis().tick_left()

plt.tight_layout()
plt.show()