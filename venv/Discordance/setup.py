import setuptools

with open("README.md", "r") as fh:
    long_description = fh.read()

setuptools.setup(
    name="Discordance-Transpiler-HENRY-ZHOU", # Replace with your own username
    version="0.0.1",
    author="Henry Zhou",
    author_email="henryz928@edu.sd45.bc.ca",
    description="A transpiler for discordance",
    long_description=long_description,
    long_description_content_type="text/markdown",
    url="https://github.com/NeverLucky123/Discordant_Transpiler",
    packages=setuptools.find_packages(),
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",
        "Operating System :: OS Independent",
    ],
    python_requires='>=3.6',
)