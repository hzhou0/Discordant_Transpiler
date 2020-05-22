import pickle
import os
import PySimpleGUI as sg
import subprocess
import sys

def resource_path(relative_path):
    """ Get absolute path to resource, works for dev and for PyInstaller """
    try:
        # PyInstaller creates a temp folder and stores path in _MEIPASS
        base_path = sys._MEIPASS
        # pyinstaller puts all executables in a single dir
        # flatten paths
        relative_path = os.path.split(relative_path)[1]
    except Exception:
        base_path = os.path.abspath(".")
    return os.path.join(base_path, relative_path)

def data_path(relative_path):
    """Get absolute path to a data file in the correct cross-platform app folder"""
    from pathlib import Path
    home = str(Path.home())
    data_dir = os.path.join(home, ".discordance")
    if not os.path.isdir(data_dir):
        os.mkdir(data_dir)
    return os.path.join(data_dir, relative_path)


class gui:
    def __init__(self):
        if not os.path.isfile(data_path("gui_data")):
            self.data = {'include_dirs': None, 'infile': None, 'outfile': None, 'mode': None}
            self.include_dirs = []
            self.infile = None
            self.outfile = None
            self.mode = None
        else:
            data_file = open(data_path("gui_data"), 'rb')
            self.data = pickle.load(data_file)
            data_file.close()
            self.include_dirs = self.data["include_dirs"]
            self.infile = self.data["infile"]
            self.outfile = self.data["outfile"]
            self.mode = self.data["mode"]

    def save_state(self, include_dirs=None, infile=None, outfile=None, mode=None):
        if include_dirs is not None:
            self.data["include_dirs"] = include_dirs
            self.include_dirs = include_dirs
        if infile is not None:
            self.data["infile"] = infile
            self.infile = infile
        if outfile is not None:
            self.data["outfile"] = outfile
            self.outfile = outfile
        if mode is not None:
            self.data["mode"] = mode
            self.mode = mode
        data_file = open(data_path("gui_data"), 'wb')
        pickle.dump(self.data, data_file)
        data_file.close()


GUI = gui()

# ------ Menu Definition ------ #
menu_def = [['Configure', ['Includes', 'Cache']],
            ['Help', ['How to Use', 'Language Documentation']], ]
go_base64 = b'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 '
sg.theme('SystemDefaultForReal')

layout = [[sg.Menu(menu_def)],
          [sg.Text("Input File", text_color="Purple")],
          [sg.InputText(GUI.infile, key='infile', enable_events=True), sg.FileBrowse()],
          [sg.Text("Std Out", text_color="Purple")],
          [sg.Output(size=(50, 20))],
          [sg.Button(key='go', image_data=go_base64, image_subsample=15,
                     button_color=("white", sg.theme_background_color())),
           sg.Combo(("transpile", "make", "run"), key='mode', default_value=GUI.mode, enable_events=True,
                    readonly=True),
           sg.Text(key='status', size=(10, 1))]]
window = sg.Window('Discordance', icon=resource_path("app.ico"), layout=layout, resizable=True)
while True:  # Event Loop
    event, values = window.read()
    if event in (None, 'Exit'):
        break
    if event == "infile":
        GUI.save_state(infile=values['infile'])
    elif event == 'mode':
        GUI.save_state(mode=values['mode'])
    elif event == 'How to Use':
        import webbrowser
        webbrowser.open("https://neverlucky123.github.io/Discordant_Transpiler/#usage")
    elif event == 'Language Documentation':
        import webbrowser
        webbrowser.open("https://neverlucky123.github.io/Discordant_Transpiler/#language-reference")
    elif event == 'Includes':
        includes = [os.path.dirname(window['infile'].get()) + " (Directory of Input File)"]
        if GUI.include_dirs:
            for i in GUI.include_dirs:
                includes.append(i)
        Listbox_size = 1 if not GUI.include_dirs else len(GUI.include_dirs) + 1
        include_layout = [[sg.Text("Transpiler will search in these directories for included header and source files",
                                   text_color="Purple")],
                          [sg.InputText(key='+', visible=False, enable_events=True),
                           sg.FolderBrowse('+', size=(3, 1), font=("", 12, ""), tooltip="Add directory"),
                           sg.Button('-', size=(3, 1), font=("", 12, ""), tooltip="Remove selected"),
                           sg.Text(key='status', size=(50, 1))],
                          [sg.Listbox(includes, key='includes', size=(100, Listbox_size))]]
        include_window = sg.Window('Includes', icon=resource_path("app.ico"), layout=include_layout, resizable=True)
        while True:
            event, values = include_window.read()
            Listbox = include_window['includes']
            if event in (None, 'Exit'):
                break
            elif event == '+':
                include_window['status'].update("")
                NewPath = include_window['+'].Get()
                if not NewPath:
                    break
                Paths = Listbox.GetListValues()
                Paths.append(NewPath)
                Listbox.Update(Paths)
                Listbox.expand(True, True, True)
                GUI.save_state(include_dirs=Paths[1:])
            elif event == '-':
                include_window['status'].update("")
                selected = Listbox.get()
                Paths = Listbox.GetListValues()
                for i in selected:
                    if i != includes[0]:
                        Paths.remove(i)
                    else:
                        include_window['status'].update("Cannot remove directory of input file", text_color="Red")
                Listbox.Update(Paths)
                GUI.save_state(include_dirs=Paths[1:])
    elif event == 'Cache':
        cache = data_path("register")
        if not os.path.isfile(cache):
            temp = open(data_path("register"), 'wb')
            pickle.dump({}, temp)
            temp.close()
        cache = os.path.getsize(cache) / (1024 * 1024)
        # truncate to a 2 decimals to improve readability
        cache = '%.2f' % cache
        cache_layout = [[sg.Text("Cache is taking up " + cache + " MB", text_color='Purple', key='size')],
                        [sg.Text("The cache stores the build timestamp of all executables.")],
                        [sg.Text(
                            "Clearing the cache would cause all files to be recompiled, increasing compile time.")],
                        [sg.Button("Clear Cache"), sg.Text(key='status', size=(10, 1))]]
        cache_window = sg.Window('Cache', icon=resource_path("app.ico"), layout=cache_layout, resizable=True)
        while True:
            event, values = cache_window.read()
            if event in (None, 'Exit'):
                break
            elif event == 'Clear Cache':
                if os.path.isfile(data_path("register")):
                    os.remove(data_path("register"))
                temp = open(data_path("register"), 'wb')
                pickle.dump({}, temp)
                temp.close()
                cache_window['status'].update("Cleared", text_color="Purple")
                cache = data_path("register")
                cache = os.path.getsize(cache) / (1024 * 1024)
                cache = '%.2f' % cache
                cache_window['size'].update("Cache is taking up " + cache + " MB", text_color='Purple')
    elif event == 'go':
        try:
            cmd = [resource_path("dist/Transpiler"), GUI.mode, GUI.infile]
            p = subprocess.run(cmd, capture_output=True, text=True, stdin=subprocess.DEVNULL)
            output, err = p.stdout, p.stderr
            if output:
                print(output)
            if err:
                print(err)
            if p.returncode == 0:
                window['status'].update("Successful", text_color="Purple")
            else:
                window['status'].update("Failed", text_color="Red")
        except Exception as e:
            print(e)
window.close()
