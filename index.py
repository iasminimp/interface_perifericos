import tkinter as tk
from tkinter import messagebox

# Cria a janela principal
root = tk.Tk()
root.title("Minha Primeira Interface")
root.geometry("600x600")

# Configuração da fonte
font_style = ("Helvetica", 220)  # Nome da fonte e tamanho

# Adiciona um rótulo com a fonte aumentada
label = tk.Label(root, text="Digite algo:", font=font_style)
label.pack(pady=10)

# Adiciona um campo de entrada com a fonte aumentada
entry = tk.Entry(root, font=font_style)
entry.pack(pady=10)

# Função para exibir a entrada do usuário
def on_button_click():
    user_input = entry.get()
    messagebox.showinfo("Entrada", f"Você digitou: {user_input}")

# Adiciona um botão com a fonte aumentada
button = tk.Button(root, text="Clique Aqui", font=font_style, command=on_button_click)
button.pack(pady=10)

# Inicia o loop da aplicação
root.mainloop()
