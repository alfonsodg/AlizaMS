# Translations

AlizaMS uses the Qt Linguist toolchain for internationalization.

## Adding a New Language

1. Create a `.ts` file:

```bash
lupdate ../../main.cpp ../../GUI/*.cpp ../../browser/*.cpp -ts alizams_es.ts
```

2. Translate strings using Qt Linguist:

```bash
linguist alizams_es.ts
```

3. Compile the `.qm` binary:

```bash
lrelease alizams_es.ts
```

## File Naming

Files follow the pattern `alizams_<locale>.ts` where `<locale>` is the
Qt locale code (e.g., `es`, `fr`, `de`, `pt_BR`, `zh_CN`).

## Build Integration

When `.ts` files are present in this directory, CMake automatically
compiles them into `.qm` files during the build.
