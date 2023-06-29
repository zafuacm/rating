import yaml
import os
import json

def read_file(file_name: str) -> str:
    with open(file_name, encoding="UTF-8") as f:
        return f.read()


# 写内容到文件

def write_file(file_name: str, content: str):
    with open(file_name, "w", encoding="UTF-8") as f:
        f.write(content)

def adjust(n):
  array = [500, 350, 250, 150, 100, 50] # sum = 1400
  sum = 1400
  for i in range(min(n, 6)):
    sum -= array[i]
  return sum

def process(list):
  last_path = list['init']
  contestants = {}
  file_list = []

  file_list.append(last_path)
  for c in list['contests']:
    file_name = c['file']
    command = f"./src/rating.out rating/{last_path} contest/{file_name} rating/{file_name}"
    print(f"command = {command}")
    os.system(command)
    last_path = file_name
    file_list.append(last_path)

  for i in range(len(file_list)):
    data = read_file(f"rating/{file_list[i]}").splitlines()
    for line in data[1::]:
      line = line.split('\t')
      old_rating = int(line[0].split('->')[0])
      new_rating = int(line[0].split('->')[1])
      perf = int(line[2])
      rank = int(line[3])
      if rank == -1:
        continue
      name = line[4]
      if name not in contestants:
        contestants[name] = {
          "handle": name,
          "organization": "",
          "rating": 0,
          "maxRating": 0,
          "history": []
        }
      adjustment = adjust(0)
      if i != 0:
        adjustment = adjust(len(contestants[name]["history"]) + 1)
        contestants[name]["history"].append({
            "contestName": c['name'],
            "oldRating": old_rating,
            "newRating": new_rating,
            "perf": perf,
            "rank": rank,
            "url": c['url'],
        })
      display_rating = new_rating - adjustment
      contestants[name]["rating"] = display_rating
      contestants[name]["maxRating"] = max(display_rating, contestants[name]["maxRating"])
      # print(line)
  return contestants

def main():
  with open('./config.yaml', 'r', encoding='utf-8') as f:
    config = yaml.load(f.read(), Loader=yaml.FullLoader)
  print(config)
  ret = json.dumps(config, ensure_ascii=False)
  write_file(f"rating/config.json", ret)

  for c in config:
    ret = json.dumps(process(c), ensure_ascii=False)
    # print(ret)
    write_file(f"rating/{c['file']}", ret)

main()
