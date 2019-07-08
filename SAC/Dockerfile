FROM microsoft/dotnet:2.1-aspnetcore-runtime AS base
WORKDIR /app
EXPOSE 80
EXPOSE 443

FROM microsoft/dotnet:2.1-sdk AS build
WORKDIR /src
COPY ["SAC/SAC.csproj", "SAC/"]
RUN dotnet restore "SAC/SAC.csproj"
COPY . .
WORKDIR "/src/SAC"
RUN dotnet build "SAC.csproj" -c Release -o /app

FROM build AS publish
RUN dotnet publish "SAC.csproj" -c Release -o /app

FROM base AS final
WORKDIR /app
COPY --from=publish /app .
ENTRYPOINT ["dotnet", "SAC.dll"]