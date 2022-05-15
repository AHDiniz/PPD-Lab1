#importando a classe, da forma mais usual
import matplotlib.pyplot as plt
import numpy as np

#definindo variáveis que vamos usar nos exemplos
def plot_times(yBar, array_size):
    xBar = ['1', '2', '4', '8', '16']

    x = np.array(range(1, len(yBar) + 1))
    y = np.array(yBar)
    z = np.polyfit(x, y, 2)
    print("Coefficients: \n", z)
    vermelho = "red"
    roxo = "purple"


    x = np.linspace(-0.4 ,len(yBar) ,100)
    y = [np.polyval(z, i) for i in x]

    plt.clf()

    bars = plt.bar(xBar, yBar, color=roxo, align='center')
    for bar in bars:
        formatter = '{value:.3f} ms'
        yval = bar.get_height()
        plt.text(bar.get_x() + bar.get_width()/2.0, yval + .15, formatter.format(value = yval), ha="center")
    plt.plot(x, y, color=vermelho, linestyle='--', linewidth=2)
    plt.title("Merge Sort Array Size " + str(array_size))
    plt.xlabel("Threads")
    plt.ylabel("Time")
    plt.savefig("merge_" + str(array_size) + ".png")

exp_1 = [2.150400, 2.284800, 1.652480, 2.423296, 3.256576]
plot_times(exp_1, 10000)

exp_2 = [233.049600, 127.454976, 98.474496, 83.798784, 95.740672]
plot_times(exp_2, 1000000)

exp_3 = [450.214144, 260.177664, 215.088128, 175.706368, 204.418560]
plot_times(exp_3, 2000000)

