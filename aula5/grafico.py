import numpy as np
import matplotlib.pyplot as plt

# Constante
vdd = 3.3

# Duty cycle de 0.1 até 1.0 (passo de 0.1)
duty = np.arange(0.1, 1.1, 0.1)

v_esperado = vdd * duty

np.random.seed(42)
v_medido = [ 0.230, 0.479, 0.739, 1.004, 1.272, 1.542, 1.814, 2.093, 2.385, 2.703]

erro = v_medido - v_esperado

# Plota gráfico
plt.figure(figsize=(8,5))
plt.plot(duty, v_esperado, 'o-', label='V_esperado')
plt.plot(duty, v_medido, 's-', label='V_medido')
plt.plot(duty, erro, 'x-', label='Erro (V)')
plt.xlabel('Duty Cycle')
plt.ylabel('Valor')
plt.title('V_esperado, V_medido e Erro (V)')
plt.legend()
plt.grid(True)
plt.show()