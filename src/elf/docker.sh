docker run -it -v "$(pwd):/codecave" --workdir /codecave codecave /bin/bash

#docker run --volume "$(pwd):/codecave" codecave -d --name codecave
#Запуск image codecave в контейнере с названием codecave и привязывает папку в которой находится этот файл в :/codecave

#-d is short for --detach, which means you just run the container and then detach from it. Essentially, you run container in the background.

#docker exec -it --workdir /codecave codecave /bin/bash
#Заходим в контейнер, используя workdir мы сразу оказываемся в папке codecave, /bin/bash - потому что в терминале пишем на баше (linux => bash)

#-it is short for --interactive + --tty. When you docker run with this command it takes you straight inside the container.
#Edit: So if you run the Docker container with -itd, it runs both the -it options and detaches you from the container. As a result, your container will still be running in the background even without any default app to run.
