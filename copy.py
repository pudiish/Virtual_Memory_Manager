import pyperclip

def write_clipboard_content_to_file(file_path):
    clipboard_content = pyperclip.paste().strip()  # Remove leading/trailing whitespace and newline characters

    with open(file_path, 'w') as file:
        file.write(clipboard_content)

# Provide the file path where you want to save the clipboard content
file_path = 'file.txt'

write_clipboard_content_to_file(file_path)
