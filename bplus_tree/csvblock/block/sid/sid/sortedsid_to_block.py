import pandas as pd
import math
import os

# 讀取CSV檔案
def read_csv(file_path):
    return pd.read_csv(file_path)

# # 取得block的大小（以bytes為單位）
# def get_block_size(block):
#     return block.memory_usage(deep=True).sum()

def split_data_into_blocks(data):
    blocks = []
    current_block = pd.DataFrame()
    current_student_id = None

    for index, row in data.iterrows():
        student_id = row['student_id']

        if current_student_id is None:
            current_student_id = student_id

        # 判斷是否需要新的block
        # 計算學號的出現次數
        student_counts = data['student_id'].value_counts()
        count = student_counts.get(student_id, 0)
        # filtered_data = data.loc[data['student_id'] == student_id]
        # # 計算佔用的記憶體大小
        # size = filtered_data.memory_usage(deep=True).sum()

        if len(current_block) >= 100 or ((student_id != current_student_id) and (count+len(current_block)>=100)):
            blocks.append(current_block)
            current_block = pd.DataFrame()

        current_block = current_block._append(row)
        current_student_id = student_id

    if not current_block.empty:
        blocks.append(current_block)

    return blocks

# 將blocks轉換為CSV檔案
def save_blocks_as_csv(blocks):
    folder_name = 'block/sid'
    if not os.path.exists(folder_name):
        os.makedirs(folder_name)

    for i, block in enumerate(blocks):
        name = block.iloc[0]['student_id']
        file_path = os.path.join(folder_name, f'{name}.csv')
        block.to_csv(file_path, index=False)


# 讀取CSV檔案
data = read_csv('sort_by_sid.csv')

# 將資料切割成blocks
blocks = split_data_into_blocks(data)

# 將blocks儲存為CSV檔案
save_blocks_as_csv(blocks)
