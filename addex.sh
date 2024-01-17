#!/bin/bash

# Specify the folder path
folder_path="./in"

# Check if the folder exists
if [ -d "$folder_path" ]; then
  # Navigate to the folder
  cd "$folder_path" || exit

  # Add ".mov" extension to all files
  for file in *; do
    mv "$file" "${file%.}".mov
  done

  echo "Extension added to all files in the folder."
else
  echo "Folder not found."
fi
