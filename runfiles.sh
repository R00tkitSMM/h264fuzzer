#!/bin/bash

# Specify the folder path
folder_path="./samples"

# Check if the folder exists
if [ -d "$folder_path" ]; then
  # Navigate to the folder
  cd "$folder_path" || exit

  # Run a command on all files
  for i in {1..100}; do
    for file in *; do
      # Your command here, using "$file" as the placeholder for the current file
      # For example, echo the file name
      for i in {1..100}; do
        echo $file
        DYLD_INSERT_LIBRARIES=../flip.dylib  ../vtdecode $file
      done
      # Or run a specific command, e.g., convert images to a different format
      # Example: convert "$file" -resize 50% "output_${file}"
    done
  done

  echo "Command executed on all files in the folder."
else
  echo "Folder not found."
fi
