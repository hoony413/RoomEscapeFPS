import pandas as pd
import os
import sys  # sys 모듈을 사용하기 위해 import 구문이 필요합니다.
from pathlib import Path

def convert_excel_sheets_to_csvs(source_dir, output_dir):
    """
    source_dir 내의 모든 .xlsx 파일을 찾아 각 시트를 개별 .csv 파일로 output_dir에 저장합니다.
    """
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    for filename in os.listdir(source_dir):
        if not filename.endswith('.xlsx'):
            continue

        excel_path = os.path.join(source_dir, filename)
        
        try:
            xls = pd.ExcelFile(excel_path)
            # 각 시트를 순회하며 별도의 CSV 파일로 저장
            for sheet_name in xls.sheet_names:
                df = pd.read_excel(xls, sheet_name=sheet_name)
    
                # --- 'No' 컬럼 수정을 위한 코드 추가 ---
                if 'No' in df.columns:
                    value = df['No'].astype(str)
                    df.insert(0, 'Index', value)
                    df['No'] = df['No'].astype(str).apply(lambda x: "(_value=" + x + ")")
           
                # --- 두번째 행에 'array' 가 포함된 경우 소괄호 씌우기--- 
                for col in df.columns:
                    cell_value = str(df.iloc[0][col])  # 두 번째 행 값 확인
                    if 'array' in cell_value.lower():  
                        # 그 아래는 소괄호 씌우기
                        df.loc[1:, col] = '(' + df.loc[1:, col].astype(str) + ')'
                    
                # 출력 파일명 SheetName.csv
                csv_filename = f"{sheet_name}.csv"
                csv_path = os.path.join(output_dir, csv_filename)
                
                # UTF-8 with BOM으로 저장하여 UE에서 한글 깨짐 방지
                df.to_csv(csv_path, index=False, encoding='utf-8-sig')
                print(f"  - Successfully converted sheet '{sheet_name}' to '{csv_path}'")

        except Exception as e:
            print(f"Error converting {excel_path}: {e}")
            
            
def main():
    if len(sys.argv) > 2:
        src = sys.argv[1]
        dest = sys.argv[2]
        if os.path.isdir(src) and os.path.isdir(dest):
            convert_excel_sheets_to_csvs(src, dest)
        else:
            print(f"Error: The specified path is not a directory: {src} or {dest}")


if __name__ == "__main__":
    main()