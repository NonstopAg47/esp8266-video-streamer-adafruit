import subprocess
import numpy as np
import ffmpeg
from pathlib import Path
from os import scandir

image_extensions = [".jpg", ".jpeg", ".png", ".tif", ".tiff"]

class Pathex:
    def scantree(self, path):
        """Recursively yield DirEntry objects for given directory."""
        for entry in scandir(path):
            if entry.is_dir(follow_symlinks=False):
                yield from self.scantree(entry.path)  # see below for Python 2.x
            else:
                yield entry
            
    def get_first_file_by_stem (self, dir_path, stem, exts=None):
        dir_path = Path (dir_path)
        stem = stem.lower()

        if dir_path.exists():
            for x in sorted(list(scandir(str(dir_path))), key=lambda x: x.name):
                if not x.is_file():
                    continue
                xp = Path(x.path)
                if xp.stem.lower() == stem and (exts is None or xp.suffix.lower() in exts):
                    return xp

        return None

    def get_image_paths(self, dir_path, image_extensions=image_extensions, subdirs=False, return_Path_class=False):
        dir_path = Path (dir_path)

        result = []
        if dir_path.exists():

            if subdirs:
                gen = self.scantree(str(dir_path))
            else:
                gen = scandir(str(dir_path))

            for x in list(gen):
                if any([x.name.lower().endswith(ext) for ext in image_extensions]):
                    result.append( x.path if not return_Path_class else Path(x.path) )
        return sorted(result)

def extract_video(input_file, output_dir, output_ext="jpg", fps=0):
    pathex = Pathex()
    
    input_file_path = Path(input_file)
    output_path = Path(output_dir)

    if not output_path.exists():
        output_path.mkdir(exist_ok=True)


    if input_file_path.suffix == '.*':
        input_file_path = pathex.get_first_file_by_stem (input_file_path.parent, input_file_path.stem)
    else:
        if not input_file_path.exists():
            input_file_path = None

    if input_file_path is None:
        print("input_file not found.")
        return
    
    for filename in pathex.get_image_paths (output_path, ['.'+output_ext]):
        Path(filename).unlink()

    job = ffmpeg.input(str(input_file_path))

    kwargs = {'pix_fmt': 'rgb24'}

    kwargs.update ({'q:v':'2'}) #highest quality for jpg

    job = job.output( str (output_path / ('%5d.'+output_ext)), **kwargs )

    try:
        job = job.run()
    except:
        print("ffmpeg fail, job commandline:" + str(job.compile()) )

extract_video("E:\F\iperov - used\DeepFaceLab_DirectX12\workspace\my projects\gentlemen.mp4", "E:\F\iperov - used\DeepFaceLab_DirectX12\workspace\my projects\dlt")

