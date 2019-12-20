FROM python:3.7.3

WORKDIR /build
ENV FLASK_APP index.py
ENV FLASK_RUN_HOST 0.0.0.0
RUN pip install -r requirements.txt
COPY requirements.txt requirements.txt
COPY . .
CMD ["flask", "run"]