import os
from dotenv import load_dotenv


print('Before Load()' , os.getenv('NTUT_MAIL_ACCOUNT'))

load_dotenv()

print('After Load()' , os.getenv('NTUT_MAIL_ACCOUNT'))
print('After Load()' , os.getenv('APPDATA'))
