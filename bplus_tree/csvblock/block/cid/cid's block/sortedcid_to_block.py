import pandas as pd
import math
import os

# 讀取CSV檔案
def read_csv(file_path):
    return pd.read_csv(file_path)

# 取得block的大小（以bytes為單位）
def get_block_size(block):
    return block.memory_usage(deep=True).sum()

def split_data_into_blocks(data):
    blocks = []
    current_block = pd.DataFrame()
    current_course_id = None

    for index, row in data.iterrows():
        course_id = row['course_id']

        if current_course_id is None:
            current_course_id = course_id

        # 判斷是否需要新的block
        if len(current_block) >= 100 or (course_id != current_course_id):
            blocks.append(current_block)
            current_block = pd.DataFrame()

        current_block = current_block.append(row)
        current_course_id = course_id

    if not current_block.empty:
        blocks.append(current_block)

    return blocks

# 將blocks轉換為CSV檔案
def save_blocks_as_csv(blocks):
    folder_name = 'block/cid'
    if not os.path.exists(folder_name):
        os.makedirs(folder_name)

    for i, block in enumerate(blocks):
        course_id = block.iloc[0]['course_id']
        student_id = block.iloc[0]['student_id']
        file_name = f'{course_id}_{student_id}.csv'
        file_path = os.path.join(folder_name, file_name)
        block.to_csv(file_path, index=False)

# 讀取CSV檔案
data = read_csv('sort_by_cid.csv')

# 將資料切割成blocks
blocks = split_data_into_blocks(data)

# 將blocks儲存為CSV檔案
save_blocks_as_csv(blocks)
