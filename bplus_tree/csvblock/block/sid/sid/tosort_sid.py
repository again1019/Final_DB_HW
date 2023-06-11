import pandas as pd

# 讀取CSV檔案
df = pd.read_csv('DB_students.csv')

# 按照學號欄位進行排序
df_sorted = df.sort_values('student_id')

# 將排序後的結果寫入新的CSV檔案
df_sorted.to_csv('sort_by_sid.csv', index=False)