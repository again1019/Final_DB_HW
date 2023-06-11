import pandas as pd

# 讀取CSV檔案
data = pd.read_csv('DB_students.csv')

# 按照課號和學號排序
sorted_data = data.sort_values(by=['course_id', 'student_id'], ascending=[True, False])

# 儲存排序後的結果
sorted_data.to_csv('sort_by_cid.csv', index=False)
