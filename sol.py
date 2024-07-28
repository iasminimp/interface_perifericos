import tkinter as tk
from tkinter import messagebox
from PIL import Image, ImageTk

# Função para exibir a entrada do usuário
def on_button_click():
    user_input = entry.get()
    messagebox.showinfo("Entrada", f"Angulação motor 1: {user_input}")

def on_button_click_dois():
    user_input_dois = entry_dois.get()
    messagebox.showinfo("Entrada", f"Angulação motor 2: {user_input_dois}")


# Cria a janela principal
root = tk.Tk()
root.title("Monitoramento de Geração de Energia Solar")
root.geometry("600x400")

# Carregar a imagem de fundo
image = Image.open("solys.jpeg")  # Substitua pelo caminho da sua imagem
bg_image = ImageTk.PhotoImage(image)

# Criar um canvas e definir a imagem de fundo
canvas = tk.Canvas(root, width=1200, height=1200)
canvas.pack(fill="both", expand=True)
canvas.create_image(200, 150, image=bg_image, anchor="nw")

# Configuração da fonte
font_style = ("Helvetica", 20)  # Nome da fonte e tamanho

# Adiciona um rótulo com a fonte aumentada ----------------------------------------------------------------------
label = tk.Label(root, text="Digite o angulo 1:", font=font_style, bg="white")
label_window = canvas.create_window(100, 100, window=label)

# Adiciona um campo de entrada com a fonte aumentada
entry = tk.Entry(root, font=font_style)
entry_window = canvas.create_window(220, 100, window=entry)

# Adiciona um botão com a fonte aumentada
button = tk.Button(root, text="Clique Aqui", font=font_style, command=on_button_click)
button_window = canvas.create_window(300, 160, window=button)


# Adiciona um rótulo com a fonte aumentada ---------------------------------------------------------------------
label_dois = tk.Label(root, text="Digite o angulo 2:", font=font_style, bg="white")
label_window_dois = canvas.create_window(300, 200, window=label_dois)

entry_dois = tk.Entry(root, font=font_style)
entry_window_dois = canvas.create_window(300, 225, window=entry_dois)

# Adiciona um botão com a fonte aumentada
button_dois = tk.Button(root, text="Clique Aqui", font=font_style, command=on_button_click_dois)
button_window_dois = canvas.create_window(300, 260, window=button_dois)

# Inicia o loop da aplicação
root.mainloop()
