<script setup lang="ts">
import { NMessageProvider, NConfigProvider, NLayout } from 'naive-ui'
</script>

<template>
  <n-config-provider>
    <n-layout>
      <div id="container">
        <n-message-provider>
          <router-view />
        </n-message-provider>
      </div>
    </n-layout>
  </n-config-provider>
</template>

<style scoped>
#app,
#container {
  height: 100vh;
  width: 100vw;
}

html,
body {
  margin: 0;
  padding: 0;
}
</style>