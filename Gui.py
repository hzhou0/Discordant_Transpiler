import pickle

import PySimpleGUI as sg
from Transpiler import *


class gui:
    def __init__(self):
        if not os.path.isfile("gui_data"):
            self.data = {'include_dirs': None, 'infile': None, 'outfile': None, 'mode': None}
            self.include_dirs = None
            self.infile = None
            self.outfile = None
            self.mode = None
        else:
            data_file = open("gui_data", 'rb')
            self.data = pickle.load(data_file)
            data_file.close()
            self.include_dirs = self.data["include_dirs"]
            self.infile = self.data["infile"]
            self.outfile = self.data["outfile"]
            self.mode = self.data["mode"]

    def save_state(self, include_dirs=None, infile=None, outfile=None, mode=None):
        if include_dirs:
            self.data["include_dirs"] = include_dirs
        if infile:
            self.data["infile"] = infile
        if outfile:
            self.data["outfile"] = outfile
        if mode:
            self.data["mode"] = mode
        data_file = open("gui_data", 'wb')
        pickle.dump(self.data, data_file)
        data_file.close()

    def construct(self):
        if not self.infile:
            std_out.append("Please specify input file\n")
        self.project = project(self.infile, self.include_dirs, self.outfile)


GUI = gui()

# ------ Menu Definition ------ #
menu_def = [['Configure'],
            ['Help', ['How to Use', 'Language Documentation']], ]
go_base64 = b'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 '
sg.theme('SystemDefaultForReal')
layout = [[sg.Menu(menu_def)],
          [sg.Text("Input File")],
          [sg.InputText(GUI.infile, key='infile', enable_events=True), sg.FileBrowse()],
          [sg.Text("Std Out")],
          [sg.Output(size=(50, 20))],
          [sg.Button(key='go', image_data=go_base64, image_subsample=15,
                     button_color=("white", sg.theme_background_color())),
           sg.Combo(("transpile", "make", "run"), key='mode', default_value=GUI.mode, enable_events=True,
                    readonly=True)]]

window = sg.Window('Discordant Transpiler', icon=resource_path("app.ico"), layout=layout, resizable=True)
while True:  # Event Loop
    event, values = window.read()
    if event in (None, 'Exit'):
        break
    if event == "infile":
        GUI.save_state(infile=values['infile'])
    elif event == 'mode':
        GUI.save_state(mode=values['mode'])
    elif event == 'go':
        cmd = [resource_path("Transpiler.py"), GUI.mode, GUI.infile]
        p = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True,
                             text=True)
        output, err = p.communicate()
        p_status = p.wait()
        if output:
            print(output)
        if err:
            print(err)
        if p_status == 0:
            print("Successful")
        # print(subprocess.run([resource_path("Transpiler.py")], capture_output=True, text=True, shell=True).stdout)

window.close()
