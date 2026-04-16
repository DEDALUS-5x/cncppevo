import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Slider
from mpl_toolkits.mplot3d import Axes3D

def get_rotation_matrix(a_deg, c_deg):
    """Calcola la matrice di rotazione combinata (Pitch e Yaw)."""
    # Conversione in radianti
    a = np.radians(a_deg)
    c = np.radians(c_deg)
    
    # Matrice Rotation X (Pitch - Asse A)
    R_x = np.array([
        [1, 0, 0],
        [0, np.cos(a), -np.sin(a)],
        [0, np.sin(a), np.cos(a)]
    ])
    
    # Matrice Rotation Z (Yaw - Asse C)
    R_z = np.array([
        [np.cos(c), -np.sin(c), 0],
        [np.sin(c), np.cos(c), 0],
        [0, 0, 1]
    ])
    
    # Combinazione: Yaw * Pitch
    return R_z @ R_x

def plot_interactive_3d(file_path):
    df = pd.read_csv(file_path, sep=None, engine='python').dropna(subset=['X', 'Y', 'Z', 't_tot'])
    
    # Se A e C mancano nel CSV, usiamo 0 (ereditarietà simulata)
    if 'A' not in df.columns: df['A'] = 0.0
    if 'C' not in df.columns: df['C'] = 0.0

    fig = plt.figure(figsize=(10, 8))
    ax = fig.add_subplot(111, projection='3d')
    plt.subplots_adjust(bottom=0.2) # Spazio per lo slider

    # Disegna la traiettoria completa in grigio chiaro per riferimento
    ax.plot(df['X'], df['Y'], df['Z'], color='gray', alpha=0.3, label='Trajectory')

    # Elementi dinamici
    point, = ax.plot([], [], [], 'ko', markersize=5) # La testina
    
    # Assi locali (X=R, Y=G, Z=B)
    scale = 10 # Lunghezza dei vettori degli assi locali
    axis_x, = ax.plot([], [], [], 'r-', linewidth=2, label='X-local')
    axis_y, = ax.plot([], [], [], 'g-', linewidth=2, label='Y-local')
    axis_z, = ax.plot([], [], [], 'b-', linewidth=2, label='Z-tool')

    # Configurazione assi
    ax.set_xlabel('X (mm)')
    ax.set_ylabel('Y (mm)')
    ax.set_zlabel('Z (mm)')
    ax.set_title('Nozzle viewer')
    ax.legend()

    # Slider
    ax_slider = plt.axes([0.2, 0.05, 0.6, 0.03])
    slider = Slider(ax_slider, 'Time', 0, len(df)-1, valinit=0, valfmt='%d')

    def update(val):
        idx = int(slider.val)
        row = df.iloc[idx]
        
        # Posizione attuale
        x, y, z = row['X'], row['Y'], row['Z']
        point.set_data([x], [y])
        point.set_3d_properties([z])

        # Calcolo rotazione assi locali
        R = get_rotation_matrix(row['A'], row['C'])
        
        # Vettori direzionali trasformati
        ux = R @ np.array([scale, 0, 0])
        uy = R @ np.array([0, scale, 0])
        uz = R @ np.array([0, 0, scale])

        # Aggiornamento grafici vettori
        axis_x.set_data([x, x + ux[0]], [y, y + ux[1]])
        axis_x.set_3d_properties([z, z + ux[2]])
        
        axis_y.set_data([x, x + uy[0]], [y, y + uy[1]])
        axis_y.set_3d_properties([z, z + uy[2]])
        
        axis_z.set_data([x, x + uz[0]], [y, y + uz[1]])
        axis_z.set_3d_properties([z, z + uz[2]])

        fig.canvas.draw_idle()

    slider.on_changed(update)
    update(0) # Inizializzazione
    plt.show()

if __name__ == "__main__":
    plot_interactive_3d("log.csv")