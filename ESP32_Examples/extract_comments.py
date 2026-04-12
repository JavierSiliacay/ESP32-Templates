import os
import re

def get_unique_chinese_comments(root_dir):
    chinese_comments = set()
    extensions = ('.ino', '.h', '.cpp', '.html', '.js')
    
    # Regex for comments
    # // ...
    # /* ... */
    single_line_comment_regex = re.compile(r'//.*[^\x00-\x7f].*')
    multi_line_comment_regex = re.compile(r'/\*[\s\S]*?[^\x00-\x7f][\s\S]*?\*/')

    for root, dirs, files in os.walk(root_dir):
        for file in files:
            if file.endswith(extensions):
                path = os.path.join(root, file)
                try:
                    with open(path, 'r', encoding='utf-8') as f:
                        content = f.read()
                        
                        # Find single line comments
                        for match in single_line_comment_regex.finditer(content):
                            chinese_comments.add(match.group().strip())
                        
                        # Find multi line comments
                        for match in multi_line_comment_regex.finditer(content):
                            # Split by lines and keep only those with Chinese
                            lines = match.group().split('\n')
                            for line in lines:
                                if any(ord(c) > 127 for c in line):
                                    chinese_comments.add(line.strip())
                except Exception as e:
                    print(f"Error reading {path}: {e}")
                    
    return chinese_comments

if __name__ == "__main__":
    root = r"c:\Users\User\Desktop\websites codebase\ESP32-Templates\ESP32_Examples"
    comments = get_unique_chinese_comments(root)
    with open("chinese_comments.txt", "w", encoding="utf-8") as f:
        for comment in sorted(list(comments)):
            f.write(comment + "\n")
    print(f"Found {len(comments)} unique Chinese comments.")
